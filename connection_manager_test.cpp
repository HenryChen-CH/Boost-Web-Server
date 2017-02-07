#include "gtest/gtest.h"
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include <boost/asio.hpp>

using namespace std;
class connection_manager_test:public ::testing::Test{
protected:
	connection_manager manager;	
};

TEST_F(connection_manager_test,start_test){
	std::map<std::string, request_handler*> handler_mapping_;
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::socket socket_(io_service_);
	connection_ptr connection_ptr1=std::make_shared<connection>(std::move(socket_), manager, handler_mapping_);
	manager.start(connection_ptr1);
	manager.start(std::make_shared<connection>(std::move(socket_), manager, handler_mapping_));
	manager.stop(connection_ptr1);
	EXPECT_EQ(manager.connection_pool_.size(),1);
	manager.stop_all();
	EXPECT_TRUE(manager.connection_pool_.empty());
}
