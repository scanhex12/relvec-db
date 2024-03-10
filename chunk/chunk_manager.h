#pragma once

#include <stddef.h>
#include <string>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/NObserver.h>
#include <Poco/Exception.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Object.h>
#include <Poco/Util/ServerApplication.h>
#include <sstream>
#include <memory>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <thread>

#include "../lib/sql/database.h"
#include "../lib/sql/scan.h"
#include "../lib/sql/marshal.h"
#include "../lib/sql/schema.h"


using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Util;

namespace NChunk {

struct ServerConfig {
    std::string masterHost;
    int masterPort;
    std::string chunkHost;
    int chunkPort;
    int freeBytes;

    ServerConfig(const std::vector<std::string>& args) {
        masterHost = args[0];
        masterPort = std::stoi(args[1]);
        chunkHost = args[2];
        chunkPort = std::stoi(args[3]);
        freeBytes = std::stoi(args[4]);
    } 
};

class ChunkManager {
public:
    ChunkManager(std::shared_ptr<ServerConfig> config, std::shared_ptr<shdb::Database> db);

    void SendHeartBeat();
    
    std::tuple<std::vector<size_t>, std::vector<size_t>, std::string> GetContent(const std::string& table);

    std::string executeQuery(const std::string& table);

    void AddRow(const std::string& table, const shdb::Row& row);

    void AddSerializedRow(const std::string& table, const std::vector<size_t>& rowSer);

    void DropTable(const std::string& table);

    void CreateTable(const std::string& table, std::string serializedSchema);

    std::string GetSerializedSchema(const std::string& table);

    ~ChunkManager();

private:
    std::thread hearbeatThread_;
    std::shared_ptr<ServerConfig> config_;
    std::shared_ptr<shdb::Database> db_;
};

}