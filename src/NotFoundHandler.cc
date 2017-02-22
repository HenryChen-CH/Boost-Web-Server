#include "NotFoundHandler.h"

const std::string not_found_html =
        "<html><body><h1>Not Found</h1></body></html>";

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return RequestHandler::OK;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request, Response* response) {
    response->SetStatus(Response::not_found);
    response->AddHeader(header_content_type_, "text/html");
    response->SetVersion("HTTP/1.0");
    response->SetBody(not_found_html);
    return RequestHandler::OK;
}
