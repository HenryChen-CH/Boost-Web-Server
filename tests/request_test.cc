#include "gtest/gtest.h"
#include "../request.hpp"
#include <string>

TEST(RequestTestToString, SimpleRequest) {
    request req;
    std::string header = "GET / HTTP/1.1\r\n";
    for (unsigned int i = 0; i < header.size(); i++) {
        req.header.push_back(header[i]);
    }
    EXPECT_EQ(req.ToString(),"GET / HTTP/1.1\r\n\r\n");
}
