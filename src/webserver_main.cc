#include <iostream>
#include "nginx-configparser/config_parser.h"
#include <boost/log/trivial.hpp>
#include "http_server.h"


int main(int argc, char* argv[]) {
    std::cout << "Server start" << std::endl;
    if (argc != 2) {
        std::cout << "Please specify config file\n";
        return 1;
    }
    NginxConfigParser parser;
    NginxConfig config;
    assert(parser.Parse(argv[1], &config) && "Parse config file fail");

    BOOST_LOG_TRIVIAL(info) << "Server is initializing.....\n";
    http_server server;
    if (server.Init(config)) {
        BOOST_LOG_TRIVIAL(info) << "Server is starting\n";
        server.start();
    };

    BOOST_LOG_TRIVIAL(error) << "Initialization failed\n";
    return 1;
}
