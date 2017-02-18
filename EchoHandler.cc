#include "EchoHandler.h"

Status EchoHandler::HandleRequest(const Request& request,
                     Response* response) {
    return OK;
}

Status EchoHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    return OK;
}
