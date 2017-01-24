#include "connection_manager.hpp"

void connection_manager::start(connection_ptr connection_ptr1) {
    connection_pool_.insert(connection_ptr1);
    connection_ptr1->start();
}

void connection_manager::stop(std::shared_ptr<connection> connection_ptr1) {
    connection_pool_.erase(connection_ptr1);
    connection_ptr1->stop();
}

void connection_manager::stop_all() {
    for (auto c: connection_pool_) {
        c->stop();
    }
    connection_pool_.clear();
}