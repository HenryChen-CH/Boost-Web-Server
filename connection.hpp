#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <memory>
#include <array>
#include <utility>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"
#include "request_parser.hpp"
#include <iostream>

class connection_manager;

class connection : public std::enable_shared_from_this<connection>{
public:
    explicit connection(boost::asio::ip::tcp::socket socket, connection_manager& manager, request_handler& handler);
    void start();
    void stop();

private:
    void read();
    void write();

    boost::asio::ip::tcp::socket socket_;
    connection_manager& connection_manager_;
    request_handler& request_handler_;
    request_parser request_parser_;
    request request_;
    response response_;

    std::array<char, 1024> buffer_;

};

typedef std::shared_ptr<connection> connection_ptr;

#endif
