#include "connection_manager.h"

void connection_manager::start(connection_ptr connection_ptr1) {
    connection_pool_.insert(connection_ptr1);
    BOOST_LOG_TRIVIAL(info) << "Establish a connection, number of current connections: " << connection_pool_.size() << "\n";
    connection_ptr1->start();
}

void connection_manager::stop(std::shared_ptr<connection> connection_ptr1) {
    connection_pool_.erase(connection_ptr1);
    BOOST_LOG_TRIVIAL(info) << "Erase a connection, remaining connections: " << connection_pool_.size() << "\n";
    connection_ptr1->stop();
}

void connection_manager::stop_all() {
    for (auto c: connection_pool_) {
        c->stop();
    }
    connection_pool_.clear();
}
