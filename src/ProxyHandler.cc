#include "ProxyHandler.h"

const std::string HOST_TOKEN = "host";
const std::string PORT_TOKEN = "port";

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request,
                     Response* response) {
    response->SetStatus(Response::ok);
    response->SetVersion("HTTP/1.0");
    response->AddHeader("Content-Type", "text/plain");
    response->SetBody(request.raw_request());

    return RequestHandler::OK;
}

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    uri_prefix_ = uri_prefix;

    // Get remote host and post from config.
    if (config.statements_.size() >= 2) {
    	for (auto const& statement: config.statements_) {
    		std::vector<std::string> tokens = statement->tokens_;
        	if (tokens.size() == 2) {
        		if (tokens.at(0) == HOST_TOKEN) {
            		host_ = tokens.at(1);
            	}
            	else if (tokens.at(0) == PORT_TOKEN) {
            		port_ = tokens.at(1);
            	}
        	}
        }
        BOOST_LOG_TRIVIAL(info) << "Init ProxyHandler " << "host: " << host_ << ", port: " << port_ << "\n";
        return OK;
    }
    BOOST_LOG_TRIVIAL(error) << "Init ProxyHandler: bad config\n";
    return NOT_ENOUGH_PARAM;
}