#include "gtest/gtest.h"
#include "response.h"
#include "header.h"
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
    myresponse.ToBuffer();
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
}

TEST_F(response_test,ToString_test_status_fail){
	myresponse.status = bad_request;
    myresponse.ToBuffer();
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 46\r\n\r\n<html><body><h1>Bad Request</h1></body></html>");
}

TEST_F(response_test,ToString_test_header){
	myresponse.status = ok;
	myresponse.headers.push_back(h);
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
}

TEST_F(response_test,ToString_test_body){
	myresponse.status = ok;
	myresponse.headers.push_back(h);
    myresponse.ToBuffer();
	copy(body.begin(), body.end(), back_inserter(myresponse.content));
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 0\r\n\r\nbody");
}
