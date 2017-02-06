#include "gtest/gtest.h"
#include "request_handler_file.h"

using namespace std;

class request_handler_file_test : public ::testing::Test{
protected:
	//request_handler_file file_handler(string dir_path,string root_url);
};

TEST_F(request_handler_file_test,path_test){
	request_handler_file file_handler("/static","/foo/bar");
	string path=file_handler.file_name_from_path(file_handler.dir_path);
	EXPECT_EQ(path,"bar");
}

TEST_F(request_handler_file_test,extension_test){
	request_handler_file file_handler("/static","/foo/bar");
	string extension=file_handler.extension("foo.html");
	EXPECT_EQ(extension,".html");
}

TEST_F(request_handler_file_test,handler_test){
	//need editing
	EXPECT_TRUE(1);
}