#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "nginx-configparser/config_parser.h"
#include <string>
#include <boost/asio.hpp>
#include <memory>
#include <map>
#include <boost/log/trivial.hpp>
#include "connection_manager.h"
#include "request_handler.h"
#include "request_handler_echo.h"
#include "request_handler_file.h"
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
    int route_block_parse(NginxConfig *config);

    FRIEND_TEST(http_server_test,port_test);
    FRIEND_TEST(http_server_test,url_test);
};

#endif
