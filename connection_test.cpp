#include "gtest/gtest.h"
#include "connection.hpp"
#include "connection_manager.hpp"
#include <boost/asio.hpp> 

using namespace std;
class connection_test:public ::testing::Test{
protected:
	//request_handler handler;
	boost::asio::io_service io_service_;
	//boost::asio::ip::tcp::socket socket_;
	//socket_(io_service_);
//	connection_manager manager;
	//connection* conn(io_service_);
	
	
};
TEST_F(connection_test,start_test){
	//need editing

	//conn.buffer_={};
	EXPECT_EQ(1,1);
}