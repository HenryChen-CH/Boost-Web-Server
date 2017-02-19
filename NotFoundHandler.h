#ifndef NOTFOUNDHANDLER_H
#define NOTFOUNDHANDLER_H

#include "RequestHandler.h"

class NotFoundHandler : public RequestHandler {
public:
    Status Init(const std::string& uri_prefix, const NginxConfig& config);
    Status HandleRequest(const Request& request, Response* response);
};

REGISTER_REQUEST_HANDLER(NotFoundHandler);

#endif
