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

#include "../lib/serialize.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Util;


class MyRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
        try {
            std::istream& requestStream = request.stream();
            std::stringstream ss;
            StreamCopier::copyStream(requestStream, ss);
            std::string requestBody = ss.str();

            Parser parser;
            Poco::Dynamic::Var result = parser.parse(requestBody);
            Object::Ptr jsonObj = result.extract<Object::Ptr>();

            auto query = jsonObj->get("query").convert<std::string>();
            std::string responseBody;
            Object::Ptr jsonRsp;
            /*
            if (query.size() % 2 == 0) {
                jsonRsp = new Object();
                std::vector<size_t> targetChunks = {1,2,3};
                auto encoded = NUtils::NSerialization::Serialize(targetChunks);
                jsonRsp->set("chunks", encoded);
            } else {
                jsonRsp = new Object();
                std::vector<size_t> targetChunks = {4,5,6};
                auto encoded = NUtils::NSerialization::Serialize(targetChunks);
                jsonRsp->set("chunks", encoded);
            }*/

            // Process query
            
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
};

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&) override {
        return new MyRequestHandler();
    }
};

class MyServerApp : public ServerApplication {
protected:
    int main(const std::vector<std::string>& args) override {
        ServerSocket svs(12346);
        HTTPServerParams* params = new HTTPServerParams();
        HTTPServer srv(new MyRequestHandlerFactory(), svs, params);
        srv.start();
        waitForTerminationRequest();
        srv.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    MyServerApp app;
    return app.run(argc, argv);
}
