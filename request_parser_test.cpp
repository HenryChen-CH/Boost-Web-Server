#include "gtest/gtest.h"
#include "request_parser.hpp"

using namespace std;
class request_parser_test:public ::testing::Test{
protected:
	request_parser parser;
	
	
};
TEST_F(request_parser_test,parse_test){
	request myrequest;
	myrequest.header={} ;   //need editing
	myrequest.body={} ;  //need editing
	char* start=new char[1];
	char* end=new char[1];
	result_type parse_result=parser.parse(myrequest,start,end);
	EXPECT_EQ(parse_result, 0);//need editing
}

//several more cases