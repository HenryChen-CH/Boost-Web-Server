#include <gtest/gtest.h>
#include <Response.h>

class Response_test:public ::testing::Test{
protected:
	Response response_;
};

TEST_F(Response_test,integrated_test){
	//Response response_;
	response_.SetStatus(Response::not_found);
	EXPECT_EQ(response_.response_code_,404);
	response_.AddHeader("header","text/html");
	EXPECT_EQ(response_.headers_["header"],"text/html");
	response_.SetBody("body");
	EXPECT_EQ(response_.body_,"body");
	response_.SetVersion("HTTP/1.1");
	EXPECT_EQ(response_.version_,"HTTP/1.1");
	std::string response_str=response_.ToString();
	EXPECT_EQ(response_str,"HTTP/1.1 404 Not Found\r\nContent-Length: 4\r\nheader: text/html\r\n\r\nbody");
}

TEST_F(Response_test,str_test){
	std::string str=response_.ToString();
	EXPECT_EQ(str,"HTTP/1.0 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
}