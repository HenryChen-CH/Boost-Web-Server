#ifndef STATICFILEHANDLER_H
#define STATICFILEHANDLER_H

#include "RequestHandler.h"

class StaticFileHandler : public RequestHandler{
public:
    Status HandleRequest(const Request& request, Response* response);
    Status Init(const std::string& uri_prefix, const NginxConfig& config);
};

REGISTER_REQUEST_HANDLER(StaticFileHandler);

#endif
