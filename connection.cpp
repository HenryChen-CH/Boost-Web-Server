#include "connection.hpp"

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
    socket_.async_read_some(boost::asio::buffer(buffer_), [this](boost::system::error_code ec, std::size_t bytes_transferred){
        if (!ec) {
            result_type result = request_parser_.parse(request_, buffer_.data(), buffer_.data()+bytes_transferred);
            if (result == good) {
                std::cout << "Package content:\n" << request_.ToString() << std::endl;

                request_.header.clear();
            } else {
                read();
            }
        }
    });
}

void connection::stop() {
    socket_.close();
}