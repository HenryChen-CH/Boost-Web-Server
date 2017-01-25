#include "response.hpp"

const std::string ok_ =
        "HTTP/1.1 200 OK\r\n";

const std::string bad_request_ =
        "HTTP/1.1 400 Bad Request\r\n";


response::response() {

}
const std::string name_value_separator = ": ";
const std::string crlf = "\r\n";

std::vector<boost::asio::const_buffer> response::ToBuffer() {
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(to_buffer(status)));
    for (int i = 0; i < headers.size(); i++) {
        header& h = headers[i];
        buffers.push_back(boost::asio::buffer(h.name));
        buffers.push_back(boost::asio::buffer(name_value_separator));
        buffers.push_back(boost::asio::buffer(h.value));
        buffers.push_back(boost::asio::buffer(crlf));
    }
    buffers.push_back(boost::asio::buffer(crlf));
    buffers.push_back(boost::asio::buffer(content));

    return buffers;
}

std::string response::ToString() {
    std::string result;
    result += to_string(status);
    for (int i = 0; i < headers.size(); i++) {
        header& h = headers[i];
        result += h.name;
        result += name_value_separator;
        result += h.value;
        result += crlf;
    }
    result += crlf;
    for (char c : content) {
        result += c;
    }

    return result;
}

std::string response::to_string(reponse_status status) {
    switch (status) {
        case ok:
            return ok_;
        default:
            return bad_request_;
    }
}

boost::asio::const_buffer response::to_buffer(reponse_status status) {
    switch (status) {
        case ok:
            return boost::asio::buffer(ok_);
        default:
            return boost::asio::buffer(bad_request_);
    }
}
