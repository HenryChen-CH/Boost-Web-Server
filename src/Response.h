#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <map>
#include <gtest/gtest_prod.h>
class Response {
public:
    enum ResponseCode {
        // Define your HTTP response codes here.
        ok = 200,
        bad_request = 400,
        not_found = 404
    };

    void SetStatus(const ResponseCode response_code);
    void AddHeader(const std::string& header_name, const std::string& header_value);
    void SetBody(const std::string& body);
    void SetVersion(const std::string& version);
    ResponseCode GetResponseCode();

    std::string ToString();

private:
    std::string body_;
    std::map<std::string, std::string> headers_;
    ResponseCode response_code_ = bad_request;
    std::string version_ = "HTTP/1.0";

    std::string to_string(ResponseCode status);
    FRIEND_TEST(NotFoundHandler_test,handle_request_test);
    FRIEND_TEST(EchoHandler_test,handle_request_test);
    FRIEND_TEST(Response_test,integrated_test);
};
#endif
