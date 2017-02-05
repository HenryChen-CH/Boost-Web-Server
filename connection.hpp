#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <memory>
#include <array>
#include <utility>
#include <map>
#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"
#include "request_parser.hpp"
#include <iostream>

class connection_manager;

class connection : public std::enable_shared_from_this<connection>{
public:
    explicit connection(boost::asio::ip::tcp::socket socket, connection_manager& manager, std::map<std::string, request_handler*>& handler_mapping_);
    void start();
    void stop();

private:
    void read();
    void write();

    boost::asio::ip::tcp::socket socket_;
    connection_manager& connection_manager_;
    std::map<std::string, request_handler*> handler_mapping_;
    request_parser request_parser_;
    request request_;
    response response_;

    std::array<char, 1024> buffer_;

};

typedef std::shared_ptr<connection> connection_ptr;

#endif
