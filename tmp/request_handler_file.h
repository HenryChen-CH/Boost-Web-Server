//
// Created by 陈昊 on 2/4/17.
//

#ifndef TEAM13_REQUEST_HANDLER_FILE_H
#define TEAM13_REQUEST_HANDLER_FILE_H

#include <string>
#include <iterator>
#include <fstream>
#include <iostream>
#include "request_handler.h"
#include "mime_types.h"
#include "gtest/gtest_prod.h"
class request_handler_file : public request_handler{
public:
    request_handler_file(std::string root_url, std::string dir_path);
    void handle_request(const request &req, response& resp) override;

private:
    std::string dir_path;
    std::string root_url;
    std::string file_name_from_path(const std::string file_path);
    std::string extension(std::string file);

    FRIEND_TEST(request_handler_file_test,path_test);
    FRIEND_TEST(request_handler_file_test,extension_test);
    FRIEND_TEST(request_handler_file_test,handler_test);
};


#endif //TEAM13_REQUEST_HANDLER_FILE_H
