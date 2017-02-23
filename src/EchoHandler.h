#ifndef ECHOHANDLER_H
#define  ECHOHANDLER_H

#include "RequestHandler.h"
#include "gtest/gtest_prod.h"
class EchoHandler : public RequestHandler {
public:
    Status HandleRequest(const Request& request, Response* response);
    Status Init(const std::string& uri_prefix, const NginxConfig& config);

private:
    std::string uri_prefix_;
    FRIEND_TEST(EchoHandler_test,init_test);
};

REGISTER_REQUEST_HANDLER(EchoHandler);

#endif
