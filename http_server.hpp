#include "nginx-configparser/config_parser.h"
#include <string>
#include <boost/asio.hpp>

class http_server {
public:
    bool Init(const std::string& config_file);
    void start();
private:
    NginxConfig config_;
    std::string port;
};
