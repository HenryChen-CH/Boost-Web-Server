#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler_echo.h"

connection::connection(boost::asio::ip::tcp::socket socket, connection_manager &manager, std::map<std::string, request_handler*>& handler_mapping)
: socket_(std::move(socket)),
  connection_manager_(manager),
  handler_mapping_(handler_mapping)
{

}

void connection::start() {
    read();
}

void connection::read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(buffer_), [this, self](boost::system::error_code ec, std::size_t bytes_transferred){
        if (!ec) {
            result_type result = request_parser_.parse(request_, buffer_.data(), buffer_.data()+bytes_transferred);
            if (result == good) {
                BOOST_LOG_TRIVIAL(info) << "Receive HTTP Packet from: " << socket_.remote_endpoint().address().to_string() << "\n";
                BOOST_LOG_TRIVIAL(info) << request_.ToString() << "\n";
                router(request_.url())->handle_request(request_, response_);
                write();
                request_.header.clear();
            } else {
                read();
            }
        } else {
            connection_manager_.stop(self);
        }
    });
}

void connection::stop() {
    socket_.close();
}

void connection::write() {
    boost::asio::async_write(socket_, response_.ToBuffer(),
    [this](boost::system::error_code ec, std::size_t){
        if (!ec) {
            BOOST_LOG_TRIVIAL(info) << "Successfully send response back to " << socket_.remote_endpoint().address().to_string() << "\n";
            BOOST_LOG_TRIVIAL(info) << "Response: \n";
            BOOST_LOG_TRIVIAL(info) << "---------Start---------: \n";
            BOOST_LOG_TRIVIAL(info) << response_.ToString();
            BOOST_LOG_TRIVIAL(info) << "----------End----------: \n";
            boost::system::error_code ignored_ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                             ignored_ec);
            BOOST_LOG_TRIVIAL(info) << "Shutdown socket " << socket_.remote_endpoint().address().to_string() << "\n";
            response_.headers.clear();
            response_.content.clear();
            return;
        }
        BOOST_LOG_TRIVIAL(error) << "Send response failed \n";
    });
}

request_handler* connection::router(std::string url) {
    size_t n = url.find('/', 1);
    std::string root = url.substr(0, n);
    if (handler_mapping_.find(root) != handler_mapping_.end()) {
        BOOST_LOG_TRIVIAL(info) << "Route: " << url << " to request handler: " << root << "\n";
        return handler_mapping_[root];
    }
    BOOST_LOG_TRIVIAL(info) << "Route " << url << " to default request handler" << "\n";
    return handler_mapping_["/"];
}
