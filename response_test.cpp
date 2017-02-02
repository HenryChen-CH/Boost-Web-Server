#include "gtest/gtest.h"
#include "response.hpp"
#include "header.hpp"
#include <string>

using namespace std;

class response_test:public ::testing::Test{
protected:
	response myresponse;
	header h = {"Content-Type", "text/plain"};
	string body = "body";
	vector<boost::asio::const_buffer> buffer;
};

TEST_F(response_test,ToString_test_status_ok){
	myresponse.status = ok;
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 200 OK\r\n\r\n");
}

TEST_F(response_test,ToString_test_status_fail){
	myresponse.status = bad_request;
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 400 Bad Request\r\n\r\n");
}

TEST_F(response_test,ToString_test_header){
	myresponse.status = ok;
	myresponse.headers.push_back(h);
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
}

TEST_F(response_test,ToString_test_body){
	myresponse.status = ok;
	myresponse.headers.push_back(h);
	copy(body.begin(), body.end(), back_inserter(myresponse.content));
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nbody");
}

TEST_F(response_test, ToBuffer_test_status) {
	myresponse.status = ok;
	string status_line = "HTTP/1.1 200 OK\r\n\r\n"
	buffer.push_back(status_line);
	EXPECT_EQ(myresponse.ToBuffer(), buffer);
}
