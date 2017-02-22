#include "gtest/gtest.h"
#include "StatusHandler.h"

class StatusHandler_test : public ::testing::Test{
protected:
	StatusHandler handler_;
};

TEST_F(StatusHandler_test,init_test){
	NginxConfig config;
	handler_.Init("foo/bar",config);
	EXPECT_EQ(handler_.uri_prefix_,"foo/bar");
}

TEST_F(StatusHandler_test,Handle_Request_test){
	EXPECT_EQ(1,1);
}
