#include "EchoHandler.h"

RequestHandler::Status EchoHandler::HandleRequest(const Request& request,
                     Response* response) {
    return OK;
}

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    return OK;
}
