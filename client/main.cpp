#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <variant>
#include <optional>

#include <cassert>
#include "../lib/serialize.h"
#include "client_config.h"
#include "client_getter.h"
#include "../lib/sql/row.h"
#include "../lib/sql/ast.h"

#include <iostream>
#include <string>
#include <random>

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;

int main(int argc, char** argv) {
    auto config = std::make_shared<ClientConfig>(ClientConfig(argc, argv));
    auto client = ClientGetter(config);
    auto table = std::to_string(rand());
    std::cout << "START TEST\n";
    client.AddTableOnMaster(table, 10);
    std::cout << "Added table on master!\n";
    auto fixed_schema = std::make_shared<shdb::Schema>(shdb::Schema{
    {"id", shdb::Type::uint64}, {"name", shdb::Type::varchar, 1024}, {"age", shdb::Type::uint64}, {"graduated", shdb::Type::boolean}});
    std::cout << "Start creating table on chunk\n";

    client.CreateTable(table, fixed_schema);
    std::cout << "End creating table on chunk\n";

    auto rows = std::vector<shdb::Row>{
        {static_cast<int64_t>(0), static_cast<int64_t>(20), std::string("Ann"), true},
        {static_cast<int64_t>(1), static_cast<int64_t>(21), std::string("Bob"), false},
        {static_cast<int64_t>(2), static_cast<int64_t>(19), std::string("Sara"), true},
        {static_cast<int64_t>(-2), static_cast<int64_t>(19), std::string("Sara"), true}};
    
    std::cout << "start adding rows in chunks\n";
    for (auto row: rows) {
        client.InsertRowOnTable(table, row);
    }
    std::cout << "adding rows in sucessfull\n";

    auto predicted = client.GetTableContent(table);
    assert(predicted == rows);
    std::cout << "Correct!\n";
}
