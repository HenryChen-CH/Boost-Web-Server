#include "connection.hpp"
#include "connection_manager.hpp"

connection::connection(boost::asio::ip::tcp::socket socket, connection_manager &manager, request_handler &handler)
: socket_(std::move(socket)),
  connection_manager_(manager),
  request_handler_(handler)
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
                std::cout << "Package content:\n" << request_.ToString() << std::endl;
                request_handler_.handle_request(request_, response_);
                std::cout << "Response: \n" << response_.ToString() << "\n";
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
            response_.headers.clear();
            response_.content.clear();
            boost::system::error_code ignored_ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                             ignored_ec);
        }
    });
}