#ifndef PROXYHANDLER_H
#define PROXYHANDLER_H

#include "RequestHandler.h"
#include "gtest/gtest_prod.h"
#include "nginx-configparser/config_parser.h"
#include <boost/log/trivial.hpp>
#include <boost/asio.hpp>

class ProxyHandler : public RequestHandler {
public:
    Status HandleRequest(const Request& request, Response* response);
    Status Init(const std::string& uri_prefix, const NginxConfig& config);

private:
    std::string uri_prefix_;
    std::string host_;
    std::string port_;
    FRIEND_TEST(ProxyHandler_test,init_test);
};

REGISTER_REQUEST_HANDLER(ProxyHandler);

#endif
