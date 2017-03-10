#include "Response.h"

const std::string name_value_separator = ": ";
const std::string crlf = "\r\n";

const std::string ok_ = "OK";
const std::string found_ = "Found";
const std::string not_found_ = "Not Found";
const std::string bad_request_ = "Bad Request";

const std::string content_length_ = "Content-Length";

void Response::SetStatus(const ResponseCode response_code) {
    response_code_ = response_code;
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value) {
    headers_[header_name] = header_value;
}

void Response::SetBody(const std::string& body) {
    body_ = body;
}

void Response::SetVersion(const std::string& version) {
    version_ = version;
}

std::string Response::ToString() {
    headers_[content_length_] = std::to_string(body_.length());

    std::string response = "";
    response.append(version_);
    response.append(" ");
    response.append(std::to_string((int)response_code_));
    response.append(" ");
    response.append(to_string(response_code_));
    response.append(crlf);
    for (auto p = headers_.begin(); p != headers_.end(); p++) {
        response.append(p->first);
        response.append(name_value_separator);
        response.append(p->second);
        response.append(crlf);
    }
    response.append(crlf);
    response.append(body_);
    return response;
}

std::string Response::to_string(ResponseCode status) {
    switch (status) {
        case ok:
            return ok_;
        case found:
            return found_;
        case not_found:
            return not_found_;
        default:
            return bad_request_;
    }
}

Response::ResponseCode Response::GetResponseCode() {
    return response_code_;
}

std::string Response::GetVersion() {
    return version_;
}
