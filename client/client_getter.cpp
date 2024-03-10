#include "client_getter.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <cassert>

#include "../lib/serialize.h"
#include "../lib/sql/marshal.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;

ClientGetter::ClientGetter(std::shared_ptr<ClientConfig> config) : config_(config) {}

std::vector<std::vector<ClientGetter::HostInfo>> ClientGetter::GetChunks(const std::vector<std::string>& tables) {
    Object::Ptr jsonObj = new Object();
    jsonObj->set("query_type", "chunks_by_query");
    jsonObj->set("num_tables", tables.size());
    for (size_t i = 0; i < tables.size(); ++i) {
        auto key = "table_" + std::to_string(i);
        jsonObj->set(key, tables[i]);
    }

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string jsonStr = oss.str();

    HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");
    req.setContentLength(jsonStr.length());

    HTTPClientSession session(config_->masterHost, config_->masterPort);

    std::ostream& os = session.sendRequest(req);
    os << jsonStr;

    HTTPResponse res;
    std::istream& is = session.receiveResponse(res);
    
    Parser parser;
    Poco::Dynamic::Var result = parser.parse(is);
    Object::Ptr jsonObjResp = result.extract<Object::Ptr>();

    std::vector<std::vector<ClientGetter::HostInfo>> answer;
    for (size_t i = 0; i < tables.size(); ++i) {
        auto key = "table_" + std::to_string(i);
        auto numHosts = jsonObjResp->get(key + "_size").convert<int>();
        answer.push_back({});
        for (int j = 0; j < numHosts; ++j) {
            auto host = jsonObjResp->get(key + "_host_" + std::to_string(j)).convert<std::string>();
            auto port = jsonObjResp->get(key + "_port_" + std::to_string(j)).convert<int>();
            std::cout << "client got chunk address " << host << ' ' << port << '\n';
            answer.back().push_back({host, port});
        }
    }
    return answer;
}

std::shared_ptr<shdb::Schema> ClientGetter::GetTableSchema(const std::string& table) {
    Object::Ptr jsonObj = new Object();
    jsonObj->set("query_type", "schema");
    jsonObj->set("table_name", table);

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string jsonStr = oss.str();

    HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");
    req.setContentLength(jsonStr.length());
    auto chunks = GetChunks({table})[0];
    for (auto chunk: chunks) {
        HTTPClientSession session(chunk.first, chunk.second);

        std::ostream& os = session.sendRequest(req);
        os << jsonStr;

        HTTPResponse res;
        std::istream& is = session.receiveResponse(res);
    
        Parser parser;
        Poco::Dynamic::Var result = parser.parse(is);
        Object::Ptr jsonObjResp = result.extract<Object::Ptr>();
        auto serializedSchema = shdb::deserializeSchema(jsonObjResp->get("serialized_schema").convert<std::string>());
        return std::make_shared<shdb::Schema>(serializedSchema);
    }
    assert(0);
}

void ClientGetter::AddTableOnMaster(std::string table, size_t tableSize) {
    Object::Ptr jsonObj = new Object();
    jsonObj->set("query_type", "insert_table");
    jsonObj->set("table", table);
    jsonObj->set("table_size", tableSize);

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string jsonStr = oss.str();

    HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");
    req.setContentLength(jsonStr.length());

    HTTPClientSession session(config_->masterHost, config_->masterPort);

    std::ostream& os = session.sendRequest(req);
    os << jsonStr;

    HTTPResponse res;
    std::istream& is = session.receiveResponse(res);
    
    Parser parser;
    Poco::Dynamic::Var result = parser.parse(is);
    Object::Ptr jsonObjResp = result.extract<Object::Ptr>();
}

void ClientGetter::RemoveTableOnMaster(std::string table) {
    Object::Ptr jsonObj = new Object();
    jsonObj->set("query_type", "remove_table");
    jsonObj->set("table", table);

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string jsonStr = oss.str();

    HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");
    req.setContentLength(jsonStr.length());

    HTTPClientSession session(config_->masterHost, config_->masterPort);

    std::ostream& os = session.sendRequest(req);
    os << jsonStr;

    HTTPResponse res;
    std::istream& is = session.receiveResponse(res);
    
    Parser parser;
    Poco::Dynamic::Var result = parser.parse(is);
    Object::Ptr jsonObjResp = result.extract<Object::Ptr>();
}

std::vector<shdb::Row> ClientGetter::GetTableContent(std::string table) {
    Object::Ptr jsonObj = new Object();
    jsonObj->set("query_type", "table_content");
    jsonObj->set("table_name", table);

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string jsonStr = oss.str();

    HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");
    req.setContentLength(jsonStr.length());
    std::vector<shdb::Row> rows;
    auto chunks = GetChunks({table})[0];
    for (auto chunk: chunks) {
        HTTPClientSession session(chunk.first, chunk.second);

        std::ostream& os = session.sendRequest(req);
        os << jsonStr;

        HTTPResponse res;
        std::istream& is = session.receiveResponse(res);
    
        Parser parser;
        Poco::Dynamic::Var result = parser.parse(is);
        Object::Ptr jsonObjResp = result.extract<Object::Ptr>();
        auto serializedRows = NUtils::NSerialization::Deserialize(jsonObjResp->get("serialized_rows"));
        auto serializedLengths = NUtils::NSerialization::Deserialize(jsonObjResp->get("serialized_length"));
        auto serializedSchema = shdb::deserializeSchema(jsonObjResp->get("serialized_schema").convert<std::string>());

        shdb::Marshal marshal(std::make_shared<shdb::Schema>(serializedSchema));
    
        size_t curIter = 0;
        std::vector<uint8_t> convertedSerializedRows(serializedRows.size());
        for (size_t i = 0; i < serializedRows.size(); ++i) {
            convertedSerializedRows[i] = serializedRows[i];
        }
        for (size_t i = 0; i < serializedLengths.size(); ++i) {
            auto row = marshal.deserializeRow(convertedSerializedRows.data() + curIter);
            curIter += serializedLengths[i];
            rows.push_back(row);
        }
    }
    return rows;
}

void ClientGetter::InsertRowOnTable(std::string table, shdb::Row row) {
    auto schema = GetTableSchema(table);
    shdb::Marshal marshal(schema);
    std::vector<uint8_t> rowData(marshal.getRowSpace(row));
    marshal.serializeRow(rowData.data(), row);
    std::vector<size_t> rowDataToConvert(rowData.size());
    for (size_t i = 0; i < rowData.size(); ++i) {
        rowDataToConvert[i] = rowData[i];
    }

    Object::Ptr jsonObj = new Object();
    jsonObj->set("query_type", "add_row");
    jsonObj->set("table_name", table);
    std::cout << "add row serialized value " << NUtils::NSerialization::Serialize(rowDataToConvert) << '\n';
    jsonObj->set("serialized_row", NUtils::NSerialization::Serialize(rowDataToConvert));

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string jsonStr = oss.str();

    HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");
    req.setContentLength(jsonStr.length());
    auto chunks = GetChunks({table})[0];
    for (auto chunk: chunks) {
        HTTPClientSession session(chunk.first, chunk.second);

        std::ostream& os = session.sendRequest(req);
        os << jsonStr;

        HTTPResponse res;
        std::istream& is = session.receiveResponse(res);
    
        Parser parser;
        Poco::Dynamic::Var result = parser.parse(is);
        Object::Ptr jsonObjResp = result.extract<Object::Ptr>();   
    }
}

void ClientGetter::CreateTable(std::string table, std::shared_ptr<shdb::Schema> schema) {
    Object::Ptr jsonObj = new Object();
    jsonObj->set("query_type", "create_table");
    jsonObj->set("table_name", table);
    jsonObj->set("serialized_schema", shdb::toString(*schema));

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string jsonStr = oss.str();

    HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");
    req.setContentLength(jsonStr.length());

    auto chunks = GetChunks({table})[0];
    for (auto chunk: chunks) {
        HTTPClientSession session(chunk.first, chunk.second);

        std::ostream& os = session.sendRequest(req);
        os << jsonStr;

        HTTPResponse res;
        std::istream& is = session.receiveResponse(res);
    
        Parser parser;
        Poco::Dynamic::Var result = parser.parse(is);
        Object::Ptr jsonObjResp = result.extract<Object::Ptr>();
    }   
}
