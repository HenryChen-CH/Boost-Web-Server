#include "nginx-configparser/config_parser.h"
#include <string>
#include <boost/asio.hpp>

class http_server {
public:
    http_server();
    bool Init(const std::string& config_file);
    void start();

private:
    void accept();

    NginxConfig config_;
    std::string port;

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};
