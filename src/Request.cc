#include "Request.h"

const std::string header_body_separator = "\r\n\r\n";
const std::string header_line_separator = "\r\n";

// parse packet, the packet must contains "\r\n\r\n"
std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
    auto request_ = std::unique_ptr<Request>(new Request());
    request_->raw_ = raw_request;
    size_t n = 0;

    n = raw_request.find(header_body_separator, 0);
    request_->body_ = raw_request.substr(n+4);
    std::string header = raw_request.substr(0, n+2);
    n = header.find(header_line_separator, 0);
    std::string status_line = header.substr(0, n);
    header = header.substr(n+header_line_separator.length());

    // parse status line
    size_t prev = 0;
    n = status_line.find(" ", prev);
    request_->method_ = status_line.substr(0, n);
    prev = n+1;
    n = status_line.find(" ", prev);
    request_->uri_ = status_line.substr(prev, n-prev);
    request_->version_ = status_line.substr(n+1);

    // parse headers
    prev = 0;
    while ((n = header.find(header_line_separator, prev)) != std::string::npos) {
        std::string line = header.substr(prev, n-prev);
        size_t tmp = line.find(":", 0);
        std::string name = line.substr(0, tmp);
        std::string value = line.substr(tmp+1);
        value = value.substr(value.find_first_not_of(' '));
        request_->headers_.push_back(std::make_pair(name, value));
        prev = n+header_line_separator.length();
    }

    return request_;
}

// append body to existing body
void Request::AppendBody(std::string body) {
    body_ += body;
    raw_ += body;
}

std::string Request::GetHeader(std::string name) {
    for (auto const& header: headers_) {
        if (header.first == name) {
            return header.second;
        }
    }
    return "";
}

std::string Request::raw_request() const {
    return raw_;
}

std::string Request::method() const {
    return method_;
}

std::string Request::uri() const {
    return uri_;
}

std::string Request::version() const {
    return version_;
}

Request::Headers Request::headers() const {
    return headers_;
}

std::string Request::body() const {
    return body_;
}
