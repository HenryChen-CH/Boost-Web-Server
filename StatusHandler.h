#ifndef STATUSHANDLER_H
#define STATUSHANDLER_H

#include "RequestHandler.h"
#include <vector>
#include <utility>
#include <sstream>
#include <map>

class StatusHandler : public RequestHandler {
public:
    Status Init(const std::string& uri_prefix, const NginxConfig& config);
    Status HandleRequest(const Request& request, Response* response);

    static void LogRequest(std::string uri, Response::ResponseCode code);
    static void LogHandler(std::string uri, std::string name);
    static std::vector<std::pair<std::string, Response::ResponseCode >> request_logs_;
    static std::map<std::string, std::string> handlers_name_;
private:
    std::string uri_prefix_;
};

REGISTER_REQUEST_HANDLER(StatusHandler);

#endif
