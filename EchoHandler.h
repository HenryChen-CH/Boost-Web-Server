#ifndef ECHOHANDLER_H
#define  ECHOHANDLER_H

#include "RequestHandler.h"

class EchoHandler : public RequestHandler {
public:
    Status HandleRequest(const Request& request, Response* response);
    Status Init(const std::string& uri_prefix, const NginxConfig& config);
};

REGISTER_REQUEST_HANDLER(EchoHandler);

#endif
