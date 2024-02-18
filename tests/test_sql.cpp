#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../lib/sql/database.h"
#include "../lib/sql/db.h"
#include "../lib/sql/interpreter.h"


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
    //REQUIRE(columns[0].name == "id" && columns[0].type == shdb::Type::uint64 && columns[0].length == 0);
    //REQUIRE(columns[1].name == "name" && columns[1].type == shdb::Type::string && columns[1].length == 0);
    //REQUIRE(columns[2].name == "nick" && columns[2].type == shdb::Type::varchar && columns[2].length == 44);
    //REQUIRE(columns[3].name == "flag" && columns[3].type == shdb::Type::boolean && columns[3].length == 0);

    interpreter.execute("DROP TABLE test_table");
    REQUIRE(!db->checkTableExists("test_table"));

}

