#ifndef COMPRESSHANDLER_H
#define COMPRESSHANDLER_H

#include "RequestHandler.h"
#include "nginx-configparser/config_parser.h"
#include <boost/log/trivial.hpp>
#include <string>

class GzipCompressHandler : RequestHandler {
public:
    Status Init(const std::string& uri_prefix, const NginxConfig& config);
    Status HandleRequest(const Request& request, Response* response);

private:

};

REGISTER_REQUEST_HANDLER(GzipCompressHandler);

#endif
