#include "StatusHandler.h"

std::vector<std::pair<std::string, Response::ResponseCode >> StatusHandler::request_logs_;
std::map<std::string, std::string> StatusHandler::handlers_name_;

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    uri_prefix_ = uri_prefix;
    return OK;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response) {
    response->SetStatus(Response::ok);
    response->SetVersion("HTTP/1.0");
    response->AddHeader(header_content_type_, "text/html");
    std::map<Response::ResponseCode ,int> response_code_;
    std::map<std::string, int> uri_;
    for (auto& p: request_logs_) {
        response_code_[p.second]++;
        uri_[p.first]++;
    }

    std::stringstream ss;
    ss << "<html>" << "<head><title>Status</title></head>" << "<body>";
    ss << "<h1>Status</h1>";
    ss << "<h2>Request Statics</h2>";
    ss << "<table border=1>";
    ss << "<tr><td>uri</td><td>n</td></tr>";
    for (auto& p: uri_) {
        ss << "<tr><td>" << p.first << "</td><td>" << p.second << "</td></tr>";
    }
    ss << "<tr><td>" << "total" << "</td><td>" << request_logs_.size() << "</td></tr>";
    ss << "</table>";
    ss << "<br>";
    ss << "<table border=1>";
    ss << "<tr><td>response code</td><td>n</td></tr>";
    for (auto& p: response_code_) {
        ss << "<tr><td>" << p.first << "</td><td>" << p.second << "</td></tr>";
    }
    ss << "<tr><td>" << "total" << "</td><td>" << request_logs_.size() << "</td></tr>";
    ss << "</table>";

    ss << "<h2>Active Handlers</h2>";
    ss << "<table border=1>";
    ss << "<tr><td>" << "uri" << "</td><td>" << "Handler Class" << "</td></tr>";
    for (auto& p: handlers_name_) {
        ss << "<tr><td>" << p.first << "</td><td>" << p.second << "</td></tr>";
    }
    ss << "</table>";

    ss << "<h2>History</h2>";
    ss << "<table border=1>";
    ss << "<tr><td>uri</td><td>response code</td></tr>";
    for (auto& p: request_logs_) {
        ss << "<tr><td>" << p.first << "</td><td>" << p.second << "</td></tr>";
    }
    ss << "</table>";
    ss << "</body></html>";
    response->SetBody(ss.str());
    return OK;
}

void StatusHandler::LogRequest(std::string uri, Response::ResponseCode code) {
    request_logs_.push_back(std::make_pair(uri, code));
}

void StatusHandler::LogHandler(std::string uri, std::string name) {
    handlers_name_[uri] = name;
}
