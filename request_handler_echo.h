//
// Created by 陈昊 on 2/4/17.
//

#ifndef TEAM13_REQUEST_HANDLER_ECHO_H
#define TEAM13_REQUEST_HANDLER_ECHO_H

#include "request_handler.hpp"
#include <string>

class request_handler_echo : public request_handler{
public:
    request_handler_echo(){};
    void handle_request(const request &req, response& resp) override;

private:
};


#endif //TEAM13_REQUEST_HANDLER_ECHO_H
