#include "gtest/gtest.h"
#include "NotFoundHandler.h"
//hn#include "nginx-configparser/config_parser.h"
#include <string>

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
	EXPECT_EQ(1,1);
}