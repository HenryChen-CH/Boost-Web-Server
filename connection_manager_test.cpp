#include "gtest/gtest.h"
#include "connection_manager.hpp"

using namespace std;
class connection_manager_test:public ::testing::Test{
protected:
	connection_manager manager;	
};

TEST_F(connection_manager_test,start_test){
	EXPECT_TRUE(1);
}