#include "gtest/gtest.h"

#include "StaticFileHandler.h"

class StaticFileHandler_test : public ::testing::Test{
	protected:
		NginxConfigParser parser;
};

TEST_F(StaticFileHandler_test,init_test){
	StaticFileHandler handler;
	NginxConfig config;
	assert(parser.Parse("./config",&config));
	handler.Init("foo/bar", *(config.statements_[1].child_block_.get());
	EXPECT_EQ(handler.uri_prefix_,"foo/bar");
	EXPECT_EQ(handler.root_,"/f");
	//EXPECT_EQ(1,1);
}


