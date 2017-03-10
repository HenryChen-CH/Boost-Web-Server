#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <memory>
#include <vector>
#include <gtest/gtest_prod.h>
class Request {
public:
    static std::unique_ptr<Request> Parse(const std::string& raw_request);

    void AppendBody(std::string body);
    std::string GetHeader(std::string name);

    std::string raw_request() const;
    std::string method() const;
    std::string uri() const;
    std::string version() const;

    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers headers() const;

    std::string body() const;

private:
    std::string method_, uri_, version_, body_, raw_;
    std::vector<std::pair<std::string, std::string>> headers_;
    FRIEND_TEST(Request_test,test1);
};

#endif
