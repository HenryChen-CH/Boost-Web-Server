//
// Created by 陈昊 on 2/4/17.
//

#ifndef TEAM13_REQUEST_HANDLER_FILE_H
#define TEAM13_REQUEST_HANDLER_FILE_H

#include <string>
#include <iterator>
#include "request_handler.hpp"

class request_handler_file : public request_handler{
public:
    request_handler_file(std::string dir_path);
    void handle_request(const request &req, response& resp) override;

private:
    std::string dir_path;
};


#endif //TEAM13_REQUEST_HANDLER_FILE_H
