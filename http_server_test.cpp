#include "gtest/gtest.h"
#include "http_server.hpp"

using namespace std;

class http_server_test : public ::testing::Test{
protected:
	http_server myserver;
};

TEST_F(http_server_test,porttest){
	EXPECT_TRUE(myserver.Init("config"));
	EXPECT_EQ(myserver.port,"3000");
}

TEST_F(http_server_test, config_file_not_exist){
	EXPECT_FALSE(myserver.Init("config1"));
}
