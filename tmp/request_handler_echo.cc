//
// Created by 陈昊 on 2/4/17.
//

#include "request_handler_echo.h"

void request_handler_echo::handle_request(const request &req, response& resp) {
    resp.content = req.header;
    resp.status = ok;
    header h;
    h.name = "Content-Type";
    h.value = "text/plain";
    resp.headers.push_back(h);
}
