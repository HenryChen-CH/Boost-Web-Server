#include "gtest/gtest.h"
#include "request.hpp"
#include <string>

using namespace std;

class request_test:public ::testing::Test{
protected:
	request myrequest;
};

TEST_F(request_test,tostringtestgood){
	string header = "GET / HTTP/1.1\r\n\r\n";
	string body = "body";
	copy(header.begin(), header.end(), back_inserter(myrequest.header));
	copy(body.begin(), body.end(), back_inserter(myrequest.body));
	EXPECT_EQ(myrequest.ToString(),"GET / HTTP/1.1\r\n\r\nbody");
}
