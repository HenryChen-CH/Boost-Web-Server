#include "gtest/gtest.h"
#include "NotFoundHandler.h"
//hn#include "nginx-configparser/config_parser.h"
#include <string>
using namespace std;
class NotFoundHandler_test : public ::testing::Test{
protected:
	NginxConfigParser parser;
	//request_handler_builders builder;
	//request_handler_file file_handler(string dir_path,string root_url);
};

TEST_F(NotFoundHandler_test, init_test){
	NginxConfig config;
	const char* config_file="config";
	parser.Parse(config_file,&config);
	NotFoundHandler handler_;
	RequestHandler::Status status=handler_.Init("foo",config);
	EXPECT_EQ(status,0);
}

TEST_F(NotFoundHandler_test,handle_request_test){
	Response resp;
	Request req;
	string header = "GET / HTTP/1.1\r\n\r\n";
	string body = "body";
	NotFoundHandler handler_;
	//copy(header.begin(), header.end(), back_inserter(req.header));
	//copy(body.begin(), body.end(), back_inserter(req.body_));
	RequestHandler::Status status=handler_.HandleRequest(req,&resp);
	EXPECT_EQ(resp.response_code_,404);
	EXPECT_EQ(status,0);
}
