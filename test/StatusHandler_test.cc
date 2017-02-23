#include "gtest/gtest.h"
#include "StatusHandler.h"

class StatusHandler_test : public ::testing::Test{
protected:
	StatusHandler handler_;
};

TEST_F(StatusHandler_test,init_test){
	NginxConfig config;
	RequestHandler::Status status=handler_.Init("foo/bar",config);
	EXPECT_EQ(handler_.uri_prefix_,"foo/bar");
	EXPECT_EQ(status,0);
}

TEST_F(StatusHandler_test,Handle_Request_test){
	Response resp;
	Request req;
	RequestHandler::Status status=handler_.HandleRequest(req,&resp);

	EXPECT_EQ(status,0);
}
