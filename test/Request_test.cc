#include "Request.h"
#include <gtest/gtest.h>

TEST(Request_test,test1){
	Request req;
	std::unique_ptr<Request> req_ptr=req.Parse("GET /f/temp.txt HTTP/1.0\r\n\r\n");
	EXPECT_EQ(req_ptr->uri(),"/f/temp.txt");
	EXPECT_EQ(req_ptr->method(),"GET");
	EXPECT_EQ(req_ptr->version(),"HTTP/1.0");
	EXPECT_EQ(req_ptr->body(),"");
}