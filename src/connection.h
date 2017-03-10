#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <memory>
#include <array>
#include <utility>
#include <map>
#include <boost/log/trivial.hpp>
#include "RequestHandler.h"
#include "Request.h"
#include "Response.h"
#include <iostream>
#include "StatusHandler.h"
#include <boost/thread/thread.hpp>
#include <exception>

class connection_manager;

class connection : public std::enable_shared_from_this<connection>{
public:
    explicit connection(boost::asio::ip::tcp::socket socket, connection_manager& manager, std::map<std::string, RequestHandler*>& handler_mapping_);
    void start();
    void stop();
    static std::string longest_prefix_matching(std::vector<std::string>& uris, std::string& uri);

private:
    void read();
    void write();
    void read_body();
    RequestHandler* router(std::string url);

    boost::asio::ip::tcp::socket socket_;
    connection_manager& connection_manager_;
    std::map<std::string, RequestHandler*> handler_mapping_;
    std::unique_ptr<Request> request_;
    Response response_;
    std::string raw_response = "";

    std::array<char, 1024> buffer_;
    boost::asio::streambuf bufstream_;
    void process_request();
};

typedef std::shared_ptr<connection> connection_ptr;

#endif
