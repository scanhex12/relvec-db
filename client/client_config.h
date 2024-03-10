#pragma once
#include <string>

struct ClientConfig {
    std::string masterHost;
    int masterPort;

    ClientConfig(int argc, char** argv) {
        masterHost = argv[1];
        masterPort = std::stoi(argv[2]);
    }

};