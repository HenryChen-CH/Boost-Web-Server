#include "response.h"

const std::string ok_ =
        "HTTP/1.1 200 OK\r\n";

const std::string bad_request_ =
        "HTTP/1.1 400 Bad Request\r\n";

const std::string not_found_ =
        "HTTP/1.1 404 Not Found\r\n";

const std::string bad_request_html =
        "<html><body><h1>Bad Request</h1></body></html>";

const std::string not_found_html =
        "<html><body><h1>Not Found</h1></body></html>";

response::response() {

}
const std::string name_value_separator = ": ";
const std::string crlf = "\r\n";

std::vector<boost::asio::const_buffer> response::ToBuffer() {
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(to_buffer(status)));
    header h;
    switch (status) {
        case bad_request:
        case not_found:
            h.name = "Content-Type";
            h.value = "text/html";
            headers.push_back(h);
            break;
        default:
            break;
    }
    switch (status) {
        case bad_request:
            content.insert(content.begin(), bad_request_html.c_str(), bad_request_html.c_str()+bad_request_html.size());
            break;
        case not_found:
            content.insert(content.begin(), not_found_html.c_str(), not_found_html.c_str()+not_found_html.size());
            break;
        default:
            break;
    }
    h.name = "Content-Length";
    h.value = std::to_string(content.size());
    headers.push_back(h);

    for (unsigned int i = 0; i < headers.size(); i++) {
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
    for (unsigned int i = 0; i < headers.size(); i++) {
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
        case not_found:
            return not_found_;
        default:
            return bad_request_;
    }
}

boost::asio::const_buffer response::to_buffer(reponse_status status) {
    switch (status) {
        case ok:
            return boost::asio::buffer(ok_);
        case not_found:
            return boost::asio::buffer(not_found_);
        default:
            return boost::asio::buffer(bad_request_);
    }
}
