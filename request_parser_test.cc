#include "gtest/gtest.h"
#include "request_parser.h"
#include "request.h"
#include <string>
#include <cstring>

using namespace std;

class request_parser_test : public ::testing::Test{
protected:
	request_parser parser;
	request req;
	string http_packet = "GET / HTTP/1.1\r\nContent-Type: text/plain\r\n\r\n";
	int len = http_packet.length();
	char *start = strdup(http_packet.c_str());
	string bad_http_packet = "GET / HTTP/1.1\r\nContent-Type: text/plain\ra\n\r\n";
	char *bad_start = strdup(bad_http_packet.c_str());
};

TEST_F(request_parser_test, parse_test_good){
	result_type result = parser.parse(req, start, start+len);
	EXPECT_EQ(result, good);
}

TEST_F(request_parser_test, parse_test_intermedia){
	result_type result = parser.parse(req, start, start+len-1);
	EXPECT_EQ(result, intermedia);
}

TEST_F(request_parser_test, parse_test_bad){
	result_type result = parser.parse(req, bad_start, bad_start+len+1);
	EXPECT_EQ(result, bad);
}
