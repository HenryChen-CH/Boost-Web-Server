#include "gtest/gtest.h"
#include "request_handler.hpp"

using namespace std;
class request_handler_test:public ::testing::Test{
protected:
	request_handler handler;
	
	
};
TEST_F(request_handler_test,handlertest){
	EXPECT_EQ(1,1);
}