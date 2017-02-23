#include "gtest/gtest.h"

#include "EchoHandler.h"


class EchoHandler_test:public ::testing::Test{
	protected:
		NginxConfigParser parser;
};

TEST_F(EchoHandler_test,init_test){
	NginxConfig config;
	const char* config_file="config";
	parser.Parse(config_file,&config);
	EchoHandler handler_;
	RequestHandler::Status status=handler_.Init("foo",config);
	EXPECT_EQ(handler_.uri_prefix_,"foo");
	EXPECT_EQ(status,0);
}

TEST_F(EchoHandler_test,handle_request_test){
	Response resp;
	Request req;
	EchoHandler handler_;
	RequestHandler::Status status=handler_.HandleRequest(req,&resp);
	EXPECT_EQ(resp.response_code_,200);
	EXPECT_EQ(status,0);
}

