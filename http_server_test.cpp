#include "gtest/gtest.h"
#include "http_server.hpp"

using namespace std;
class http_server_test:public ::testing::Test{
protected:
	http_server myserver;
	
	
};
TEST_F(http_server_test,porttest){
	myserver.Init("config");
	EXPECT_EQ(myserver.port,"3000");
}