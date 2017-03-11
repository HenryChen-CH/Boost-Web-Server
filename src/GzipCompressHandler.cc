#include "GzipCompressHandler.h"

RequestHandler::Status GzipCompressHandler::HandleRequest(const Request& request, Response* response) {

}

RequestHandler::Status GzipCompressHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return OK;
}
