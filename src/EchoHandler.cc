#include "EchoHandler.h"

RequestHandler::Status EchoHandler::HandleRequest(const Request& request,
                     Response* response) {
    response->SetStatus(Response::ok);
    response->SetVersion("HTTP/1.0");
    response->AddHeader("Content-Type", "text/plain");
    response->SetBody(request.raw_request());

    return RequestHandler::OK;
}

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    uri_prefix_ = uri_prefix;
    return OK;
}
