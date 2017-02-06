#include "gtest/gtest.h"
#include "http_server.hpp"

using namespace std;

class http_server_test : public ::testing::Test{
protected:
	http_server myserver;
};

TEST_F(http_server_test,port_test){
	EXPECT_TRUE(myserver.Init("config"));
	EXPECT_EQ(myserver.port,"3000");
}

TEST_F(http_server_test, config_file_not_exist){
	EXPECT_FALSE(myserver.Init("config1"));
}

TEST_F(http_server_test,url_test){
	int type=myserver.route_block_parse(&myserver.config_);

	EXPECT_EQ(type,2);
}