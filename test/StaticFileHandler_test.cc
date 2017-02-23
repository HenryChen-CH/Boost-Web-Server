#include "gtest/gtest.h"

#include "StaticFileHandler.h"

class StaticFileHandler_test : public ::testing::Test{
	protected:
		NginxConfigParser parser;
};

TEST_F(StaticFileHandler_test,all_test){
	StaticFileHandler handler;
	NginxConfig config;
	assert(parser.Parse("./config",&config));
	handler.Init("/f", *(config.statements_[1]->child_block_.get()));

	EXPECT_EQ(handler.uri_prefix_,"/f");
	EXPECT_EQ(handler.root_,"./foo/bar");
	Request req;
	std::unique_ptr<Request> req_ptr=req.Parse("GET /f/temp.txt HTTP/1.1\r\n\r\n");
	Response resp;
	//attention here!!!
	handler.HandleRequest(*req_ptr,&resp);
	EXPECT_EQ(resp.ToString(),"HTTP/1.0 200 OK\r\nContent-Length: 9\r\nContent-Type: text/plain\r\n\r\nsomething");
}



