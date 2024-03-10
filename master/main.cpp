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

#include "../lib/serialize.h"
#include "balancer.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Util;

struct ServerConfig {
    std::string masterHost;
    int masterPort;

    ServerConfig(const std::vector<std::string>& args) {
        masterHost = args[0];
        masterPort = std::stoi(args[1]);
    } 
};

class MasterRequestHandler : public HTTPRequestHandler {
public:
    MasterRequestHandler(std::shared_ptr<Balancer> balancer): balancer_(balancer) {}

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

            if (query_type == "hearbeat") {
                jsonRsp = new Object();
                auto freeBytes = jsonObj->get("free_bytes").convert<int>();
                auto chunkPort = jsonObj->get("chunk_port").convert<int>();
                auto chunkHost = jsonObj->get("chunk_host").convert<std::string>();
                balancer_->OnUpdate({chunkHost, chunkPort}, freeBytes);
                jsonRsp->set("status", "OK");

            } else if (query_type == "chunks_by_query") {
                auto num_tables = jsonObj->get("num_tables").convert<int>();
                jsonRsp = new Object();

                for (size_t i = 0; i < num_tables; ++i) {
                    auto commonKey = "table_" + std::to_string(i);
                    auto table_i = jsonObj->get(commonKey).convert<std::string>();
                    std::cout << "query to balancer\n";
                    auto resultPartition = balancer_->GetPartition(table_i);
                    std::cout << "end query to balancer\n";
                    jsonRsp->set(commonKey + "_size", resultPartition.size());
                    for (size_t j = 0; j < resultPartition.size(); ++j) {
                        jsonRsp->set(commonKey + "_host_" + std::to_string(j), resultPartition[j].first.first);
                        jsonRsp->set(commonKey + "_port_" + std::to_string(j), resultPartition[j].first.second);
                    }
                }
            } else if (query_type == "insert_table") {
                std::cout << "GOT REQUEST INSERT TABLE\n";
                auto table = jsonObj->get("table").convert<std::string>();
                auto tableSize = jsonObj->get("table_size").convert<int>();
                jsonRsp = new Object();
                jsonRsp->set("status", "OK");
                std::cout << "try insert\n";
                balancer_->InsertTable(table, tableSize);
                std::cout << "OK\n";

            } else if (query_type == "remove_table") {
                auto table = jsonObj->get("table").convert<std::string>();
                jsonRsp = new Object();
                balancer_->EraseTable(table);
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
    std::shared_ptr<Balancer> balancer_;
};

class MasterRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    MasterRequestHandlerFactory(std::shared_ptr<Balancer> balancer): balancer_(balancer) {}

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&) override {
        return new MasterRequestHandler(balancer_);
    }
private:
    std::shared_ptr<Balancer> balancer_;
};

class MasterServerApp : public ServerApplication {
protected:
    int main(const std::vector<std::string>& args) override {
        auto config = std::make_shared<ServerConfig>(args);
        auto balancer = std::make_shared<Balancer>("balancer_state.txt");

        ServerSocket svs(config->masterPort);
        HTTPServerParams* params = new HTTPServerParams();
        HTTPServer srv(new MasterRequestHandlerFactory(balancer), svs, params);
        srv.start();
        waitForTerminationRequest();
        srv.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    MasterServerApp app;
    return app.run(argc, argv);
}
