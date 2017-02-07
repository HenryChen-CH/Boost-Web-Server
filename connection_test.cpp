#include "gtest/gtest.h"
#include "connection.hpp"
#include "connection_manager.hpp"
#include <boost/asio.hpp> 

using namespace std;
class connection_test:public ::testing::Test{
protected:
	
	
	
};
TEST_F(connection_test,start_test){
	//need editing
	std::map<std::string, request_handler*> handler_mapping_;
	//request_handler handler;
	
	//conn.buffer_={};
	EXPECT_EQ(1,1);
}