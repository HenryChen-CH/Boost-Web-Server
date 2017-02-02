#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <boost/asio.hpp>
#include <set>
#include <memory>
#include "connection.hpp"
#include "googletest/googletest/include/gtest/gtest_prod.h"

class connection_manager {
public:
    void start(connection_ptr connection_ptr1);
    void stop(std::shared_ptr<connection> connection_ptr1);
    void stop_all();

private:
	FRIEND_TEST(connection_manager_test,start_test);
    std::set<connection_ptr> connection_pool_;
};

#endif
