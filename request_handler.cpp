#include "request_handler.hpp"

request_handler::request_handler() {

}

void request_handler::handle_request(const request &req, response& resp) {
    resp.content = req.header;
    resp.status = ok;
    header h;
    h.name = "Content-Type";
    h.value = "text/plain";
    resp.headers.push_back(h);
    h.name = "Content-Length";
    h.value = std::to_string(req.header.size());
    resp.headers.push_back(h);
}