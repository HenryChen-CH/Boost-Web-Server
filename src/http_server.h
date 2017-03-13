#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "nginx-configparser/config_parser.h"
#include <string>
#include <boost/asio.hpp>
#include <memory>
#include <map>
#include <boost/log/trivial.hpp>
#include "connection_manager.h"
#include "RequestHandler.h"
#include "googletest/googletest/include/gtest/gtest_prod.h"
#include "StatusHandler.h"
#include <exception>


class http_server {
public:
    http_server();
    bool Init(NginxConfig& config);
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
    boost::asio::ssl::context context_;

    connection_manager connection_manager_;
    std::map<std::string, RequestHandler*> handler_mapping_;

    FRIEND_TEST(http_server_test,port_test);
    FRIEND_TEST(http_server_test,url_test);
};

#endif
