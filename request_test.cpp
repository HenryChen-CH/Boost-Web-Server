#include "gtest/gtest.h"
#include "request.hpp"

using namespace std;
class request_test:public ::testing::Test{
protected:
	request myrequest;
		
};
TEST_F(request_test,tostringtest){
	myrequest.header={'a','b'};
	myrequest.body={'c','d'};
	EXPECT_EQ(myrequest.ToString(),"abcd");
}