#include "http_server.hpp"

bool http_server::Init(const std::string& config_file) {
    NginxConfigParser parser_;
    if (!parser_.Parse(config_file.data(), &config_)) {
        return false;
    }
    std::vector<std::string> tokens = config_.statements_[0].get()->child_block_.get()->statements_[0].get()->tokens_;
    for (unsigned int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "listen") {
            port = tokens[i+1];
            break;
        }
    }
    return true;
}

void http_server::start() {

}
