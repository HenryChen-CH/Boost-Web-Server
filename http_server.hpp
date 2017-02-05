#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "nginx-configparser/config_parser.h"
#include <string>
#include <boost/asio.hpp>
#include <memory>
#include <map>
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include "request_handler_echo.h"
#include "googletest/googletest/include/gtest/gtest_prod.h"


class http_server {
public:
    http_server();
    bool Init(const std::string& config_file);
    void start();

private:
    void accept();
    void stop();

    NginxConfig config_;
    std::string port;

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::signal_set signals_;

    connection_manager connection_manager_;
    std::map<std::string, request_handler*> handler_mapping_;

    FRIEND_TEST(http_server_test,porttest);
};

#endif
