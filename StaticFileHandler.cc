#include "StaticFileHandler.h"

Status StaticFileHandler::HandleRequest(const Request& request,
                     Response* response) {
    return OK;
}

Status StaticFileHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    return OK;
}
