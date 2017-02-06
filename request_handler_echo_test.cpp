#include "gtest/gtest.h"
#include "request_handler_echo.h"
#include "response.hpp"

using namespace std;
class request_handler_test:public ::testing::Test{
protected:
	request_handler_echo handler;
	
	
};

TEST_F(request_handler_test,normal){
	response resp2;
	request req2;
	string header = "GET / HTTP/1.1\r\n\r\n";
	string body = "body";
	copy(header.begin(), header.end(), back_inserter(req2.header));
	copy(body.begin(), body.end(), back_inserter(req2.body));
	handler.handle_request(req2,resp2);
	string responsestr=resp2.ToString();
	EXPECT_EQ(responsestr,"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 18\r\n\r\nGET / HTTP/1.1\r\n\r\n");
}
