#include "StaticFileHandler.h"

RequestHandler::Status StaticFileHandler::HandleRequest(const Request& request,
                     Response* response) {
    return OK;
}

RequestHandler::Status StaticFileHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    return OK;
}
