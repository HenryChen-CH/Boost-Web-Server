#include "gtest/gtest.h"
#include "response.hpp"

using namespace std;
class response_test:public ::testing::Test{
protected:
	response myresponse;

};
TEST_F(response_test,ToString_test){
	myresponse.headers={};
	myresponse.status=ok;
	EXPECT_EQ(myresponse.ToString(),"HTTP/1.1 200 OK\r\n\r\n");
}
