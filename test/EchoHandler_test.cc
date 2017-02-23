#include "gtest/gtest.h"

#include "EchoHandler.h"


class EchoHandler_test:public ::testing::Test{
	protected:
		NginxConfigParser parser;
};

TEST_F(EchoHandler_test,init_test){
	EXPECT_TRUE(1);
}

