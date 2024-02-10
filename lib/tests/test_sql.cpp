#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../sql/ast.h"
#include "../sql/interpreter.h"

#define ASSERT_EQ(a, b) REQUIRE(a == b)

namespace {
auto fixed_schema = std::make_shared<shdb::Schema>(shdb::Schema{
    {"id", shdb::Type::uint64}, {"name", shdb::Type::varchar, 1024}, {"age", shdb::Type::uint64}, {"graduated", shdb::Type::boolean}});

std::shared_ptr<shdb::Database> createDatabase(int frame_count)
{
    auto db = shdb::connect("./mydb", frame_count);
    if (db->checkTableExists("test_table"))
        db->dropTable("test_table");

    db->createTable("test_table", fixed_schema);
    return db;
}
}

TEST_CASE("sql_ddl") {
    auto db = createDatabase(1);
    REQUIRE(db->checkTableExists("test_table"));

    auto interpreter = shdb::Interpreter(db);
    interpreter.execute("DROP TABLE test_table");
    REQUIRE(!db->checkTableExists("test_table"));

    interpreter.execute("CREATE TABLE test_table (id uint64, name string, nick varchar(44), flag boolean)");
    REQUIRE(db->checkTableExists("test_table"));

    auto schema = db->findTableSchema("test_table");
    REQUIRE(schema);

    auto & columns = *schema;
    REQUIRE(columns.size() == 4);
    REQUIRE((columns[0].name == "id" && columns[0].type == shdb::Type::uint64 && columns[0].length == 0));
    REQUIRE((columns[1].name == "name" && columns[1].type == shdb::Type::string && columns[1].length == 0));
    REQUIRE((columns[2].name == "nick" && columns[2].type == shdb::Type::varchar && columns[2].length == 44));
    REQUIRE((columns[3].name == "flag" && columns[3].type == shdb::Type::boolean && columns[3].length == 0));

    interpreter.execute("DROP TABLE test_table");
    REQUIRE(!db->checkTableExists("test_table"));

}

void populate(shdb::Interpreter & interpreter) {
    interpreter.execute("DROP TABLE test_table");
    interpreter.execute("CREATE TABLE test_table (id int64, age int64, name string, girl boolean)");
    interpreter.execute("INSERT test_table VALUES (0, 10+10, \"Ann\", 1>0)");
    interpreter.execute("INSERT test_table VALUES (1, 10+10+1, \"Bob\", 1<0)");
    interpreter.execute("INSERT test_table VALUES (2, 10+9, \"Sara\", 1>0)");
    interpreter.execute("INSERT test_table VALUES (-2, 10+9, \"Sara\", 1>0)");
}

TEST_CASE("sql_insert") {
    auto db = createDatabase(1);
    auto interpreter = shdb::Interpreter(db);
    populate(interpreter);

    auto rows = std::vector<shdb::Row>{
        {static_cast<int64_t>(0), static_cast<int64_t>(20), std::string("Ann"), true},
        {static_cast<int64_t>(1), static_cast<int64_t>(21), std::string("Bob"), false},
        {static_cast<int64_t>(2), static_cast<int64_t>(19), std::string("Sara"), true},
        {static_cast<int64_t>(-2), static_cast<int64_t>(19), std::string("Sara"), true}};

    size_t index = 0;
    auto table = db->getTable("test_table");
    auto scan = shdb::Scan(table);
    for (auto it = scan.begin(), end = scan.end(); it != end; ++it)
    {
        auto row = it.getRow();
        if (!row.empty())
        {
            REQUIRE(row == rows[index]);
            ++index;
        }
    }

    REQUIRE(index == rows.size());
}


TEST_CASE("Select")
{
    auto db = createDatabase(1);
    auto interpreter = shdb::Interpreter(db);
    populate(interpreter);

    auto check = [&](const std::vector<shdb::Row> & rows, const shdb::RowSet & rowset)
    {
        REQUIRE(rows.size() == rowset.getRows().size());

        for (size_t index = 0; index < rows.size(); ++index)
            REQUIRE(rows[index] == rowset.getRows()[index]);
    };

    auto rows1 = std::vector<shdb::Row>{
        {static_cast<int64_t>(0), static_cast<int64_t>(20), std::string("Ann"), true},
        {static_cast<int64_t>(1), static_cast<int64_t>(21), std::string("Bob"), false},
        {static_cast<int64_t>(2), static_cast<int64_t>(19), std::string("Sara"), true}};
    auto result1 = interpreter.execute("SELECT * FROM test_table");
    check(rows1, result1);

    auto rows2 = std::vector<shdb::Row>{
        {static_cast<int64_t>(0), static_cast<int64_t>(20), std::string("Ann"), true},
        {static_cast<int64_t>(2), static_cast<int64_t>(19), std::string("Sara"), true}};
    auto result2 = interpreter.execute("SELECT * FROM test_table WHERE age <= 20");
    check(rows2, result2);

    auto rows3 = std::vector<shdb::Row>{
        {static_cast<int64_t>(1), std::string("Ann"), true},
        {static_cast<int64_t>(2), std::string("Bob"), true},
        {static_cast<int64_t>(3), std::string("Sara"), false}};
    auto result3 = interpreter.execute("SELECT id+1, name, id < 2 FROM test_table");
    check(rows3, result3);

    auto rows4 = std::vector<shdb::Row>{{std::string("Bob"), static_cast<int64_t>(2)}, {std::string("Sara"), static_cast<int64_t>(4)}};
    auto result4 = interpreter.execute("SELECT name, id*2 FROM test_table WHERE id > 0");
    check(rows4, result4);
}

