#include <iostream>
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
#include <string>
#include <thread>
#include <chrono>
#include <tuple>

#include "../lib/serialize.h"
#include "chunk_manager.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Util;


class ChunkRequestHandler : public HTTPRequestHandler {
public:
    ChunkRequestHandler(std::shared_ptr<NChunk::ChunkManager> chunkManager): chunkManager_(chunkManager){
    }

    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
        try {
            std::istream& requestStream = request.stream();
            std::stringstream ss;
            StreamCopier::copyStream(requestStream, ss);
            std::string requestBody = ss.str();

            Parser parser;
            Poco::Dynamic::Var result = parser.parse(requestBody);
            Object::Ptr jsonObj = result.extract<Object::Ptr>();
            std::string responseBody;
            Object::Ptr jsonRsp;

            auto query_type = jsonObj->get("query_type").convert<std::string>();
            if (query_type == "chunk_info") {
                auto query = jsonObj->get("query").convert<std::string>();
                auto rsp = chunkManager_->executeQuery(query);
                jsonRsp = new Object();
                jsonRsp->set("result", rsp);
            } else if (query_type == "table_content") {
                auto table_name = jsonObj->get("table_name").convert<std::string>();
                auto rsp = chunkManager_->GetContent(table_name);
                jsonRsp = new Object();
                jsonRsp->set("serialized_rows", NUtils::NSerialization::Serialize(std::get<0>(rsp)));
                jsonRsp->set("serialized_length", NUtils::NSerialization::Serialize(std::get<1>(rsp)));
                jsonRsp->set("serialized_schema", std::get<2>(rsp));
            } else if (query_type == "add_row") {
                std::cout << "add rows!\n";
                auto table_name = jsonObj->get("table_name").convert<std::string>();
                std::cout << "got table name! " << table_name << '\n';
                auto serializedRow = NUtils::NSerialization::Deserialize(jsonObj->get("serialized_row").convert<std::string>());
                std::cout << "serialized " << jsonObj->get("serialized_row").convert<std::string>() << '\n';
                jsonRsp = new Object();
                std::cout << "chunk add row request " << jsonObj->get("serialized_row").convert<std::string>() << '\n';
                chunkManager_->AddSerializedRow(table_name, serializedRow);
            } else if (query_type == "create_table") {
                auto table_name = jsonObj->get("table_name").convert<std::string>();
                auto serialized_schema = jsonObj->get("serialized_schema").convert<std::string>();
                jsonRsp = new Object();
                std::cout << "start execute create query on chunk " << table_name << '\n';
                chunkManager_->CreateTable(table_name, serialized_schema);
            } else if (query_type == "schema") {
                auto table_name = jsonObj->get("table_name").convert<std::string>();
                auto rsp = chunkManager_->GetSerializedSchema(table_name);
                jsonRsp = new Object();
                jsonRsp->set("serialized_schema", rsp);
            }

            std::ostringstream oss;
            jsonRsp->stringify(oss);
            responseBody = oss.str();
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            response.sendBuffer(responseBody.c_str(), responseBody.length());
        } catch (JSONException& e) {
            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
            response.send();
        }
    }
private:
    std::shared_ptr<NChunk::ChunkManager> chunkManager_;
};

class ChunkRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    ChunkRequestHandlerFactory(std::shared_ptr<NChunk::ChunkManager> chunkManager):chunkManager_(chunkManager){
    }

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&) override {
        return new ChunkRequestHandler(chunkManager_);
    }
private:
    std::shared_ptr<NChunk::ChunkManager> chunkManager_;
};

class ChunkServerApp : public ServerApplication {
protected:
    int main(const std::vector<std::string>& args) override {
        auto config = std::make_shared<NChunk::ServerConfig>(args);
        auto db = shdb::connect("./mydb", 10000);

        auto chunkManager = std::make_shared<NChunk::ChunkManager>(config, db);
        ServerSocket svs(config->chunkPort);
        HTTPServerParams* params = new HTTPServerParams();
        HTTPServer srv(new ChunkRequestHandlerFactory(chunkManager), svs, params);

        std::thread hearbeatThread([chunkManager]() {
            while (true) {
                chunkManager->SendHeartBeat();
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        });

        srv.start();
        waitForTerminationRequest();
        srv.stop();
        hearbeatThread.join();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    ChunkServerApp app;
    return app.run(argc, argv);
}
