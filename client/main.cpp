#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include "../lib/serialize.h"

#include <iostream>

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;

std::vector<size_t> getChunkPorts(int masterPort, const std::string& query) {
    Object::Ptr jsonObj = new Object();
    jsonObj->set("query", query);

    std::ostringstream oss;
    jsonObj->stringify(oss);
    std::string jsonStr = oss.str();

    HTTPRequest req(HTTPRequest::HTTP_GET, "/endpoint", HTTPMessage::HTTP_1_1);
    req.setContentType("application/json");
    req.setContentLength(jsonStr.length());

    HTTPClientSession session("localhost", masterPort);

    std::ostream& os = session.sendRequest(req);
    os << jsonStr;

    HTTPResponse res;
    std::istream& is = session.receiveResponse(res);
    
    Parser parser;
    Poco::Dynamic::Var result = parser.parse(is);
    Object::Ptr jsonObjResp = result.extract<Object::Ptr>();
    
    auto chunks = jsonObjResp->get("chunks").convert<std::string>();
    std::cout << chunks.size()  << '\n' << chunks << std::endl;
    return NUtils::NSerialization::Deserialize(chunks);
}

int main() {
    auto chunks = getChunkPorts(12346, "");
    std::cout << "CHUNK SIZE : " << chunks.size() << '\n';
    for (auto elem : chunks) {
        std::cout << elem << '\n';
    }
}
