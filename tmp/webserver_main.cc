#include <iostream>
#include "http_server.h"
#include <boost/log/trivial.hpp>


int main(int argc, char* argv[]) {
    std::cout << "Server start" << std::endl;
    if (argc != 2) {
        std::cout << "Please specify config file\n";
        return 1;
    }
    http_server server;
    BOOST_LOG_TRIVIAL(info) << "Server is initializing.....\n";
    if (server.Init(std::string(argv[1]))) {
        BOOST_LOG_TRIVIAL(info) << "Server is starting\n";
        server.start();
    };

    BOOST_LOG_TRIVIAL(error) << "Initialization failed\n";
    return 1;
}
