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

TEST_F(request_handler_file_test,handler_test1){
	//need editing
	request_handler_file file_handler("/static","./foo/bar");//
	response resp;
	request req;
	string header = "GET /static/temp.txt HTTP/1.1\r\n\r\n";
	string body = "body";
	copy(header.begin(), header.end(), back_inserter(req.header));
	copy(body.begin(), body.end(), back_inserter(req.body));
	file_handler.handle_request(req,resp);
	EXPECT_EQ(resp.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nsomething");
}
TEST_F(request_handler_file_test,handler_test2){
	//need editing
	request_handler_file file_handler("/static","./foo/bar");//
	response resp;
	request req;
	string header = "GET /static/a.txt HTTP/1.1\r\n\r\n";
	string body = "body";
	copy(header.begin(), header.end(), back_inserter(req.header));
	copy(body.begin(), body.end(), back_inserter(req.body));
	file_handler.handle_request(req,resp);
	EXPECT_EQ(resp.ToString(),"HTTP/1.1 404 Not Found\r\n\r\n");
}
