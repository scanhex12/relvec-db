#include "chunk_manager.h"

namespace NChunk {
    ChunkManager::ChunkManager(std::shared_ptr<ServerConfig> config, std::shared_ptr<shdb::Database> db) : config_(config), db_(db) {
    }

    void ChunkManager::SendHeartBeat() {
        Object::Ptr jsonObj = new Object();
        jsonObj->set("query_type", "hearbeat");
        jsonObj->set("free_bytes", config_->freeBytes);
        jsonObj->set("chunk_host", config_->chunkHost);
        jsonObj->set("chunk_port", config_->chunkPort);

        std::ostringstream oss;
        jsonObj->stringify(oss);
        std::string jsonStr = oss.str();

        HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
        req.setContentType("application/json");
        req.setContentLength(jsonStr.length());
        std::cout << "request prepared\n";
        HTTPClientSession session(config_->masterHost, config_->masterPort);
        std::cout << "session started\n";

        std::ostream& os = session.sendRequest(req);
        os << jsonStr;

        std::cout << "request sended\n";

        HTTPResponse res;
        std::istream& is = session.receiveResponse(res);
        
        Parser parser;
        Poco::Dynamic::Var result = parser.parse(is);
        Object::Ptr jsonObjResp = result.extract<Object::Ptr>();

    }

    ChunkManager::~ChunkManager() {}

    std::string ChunkManager::executeQuery(const std::string& table) {
        return "some query result " + table;
    }

    void ChunkManager::AddRow(const std::string& table, const shdb::Row& row) {
        db_->getTable(table)->insertRow(row);
    }

    void ChunkManager::DropTable(const std::string& table) {
        db_->dropTable(table);
    }

    void ChunkManager::AddSerializedRow(const std::string& table, const std::vector<size_t>& rowSer) {
        shdb::Marshal marshal(db_->findTableSchema(table));
        std::vector<uint8_t> serializedRow(rowSer.size());
        for (size_t i = 0; i < rowSer.size(); ++i) {
            serializedRow[i] = rowSer[i];
        }
        auto row = marshal.deserializeRow(serializedRow.data());
        AddRow(table, row);
    }

    
    std::tuple<std::vector<size_t>, std::vector<size_t>, std::string> ChunkManager::GetContent(const std::string& table) {
        auto tableEngine = db_->getTable(table);
        auto scan = shdb::Scan(tableEngine);
        shdb::Marshal marshal(db_->findTableSchema(table));
        std::vector<size_t> response;
        std::vector<size_t> lengths;
        for (auto it = scan.begin(), end = scan.end(); it != end; ++it)
        {
            auto row = it.getRow();
            size_t serializedSize = marshal.getRowSpace(row);
            std::vector<uint8_t> serializedData(serializedSize);
            marshal.serializeRow(serializedData.data(), row);
            for (auto elem: serializedData) {
                response.push_back(elem);
            }
            lengths.push_back(serializedSize);
        }
        return {response, lengths, shdb::toString(*db_->findTableSchema(table))};
    }

    void ChunkManager::CreateTable(const std::string& table, std::string serializedSchema) {
        std::cout << serializedSchema << '\n';
        auto schema = shdb::deserializeSchema(serializedSchema);
        std::cout << "deserialized schema\n";
        std::cout << schema[0].name << ' ' << schema[1].name << ' ' << schema[2].name << ' ' << schema[3].name << '\n';
        db_->createTable(table, std::make_shared<shdb::Schema>(schema));
        std::cout << "done create table\n";
    }

    std::string ChunkManager::GetSerializedSchema(const std::string& table) {
        shdb::toString(*db_->findTableSchema(table));
    }

}
