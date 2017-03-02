#include "connection.h"
#include "connection_manager.h"

connection::connection(boost::asio::ip::tcp::socket socket, connection_manager &manager, std::map<std::string, RequestHandler*>& handler_mapping)
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
            result_type result = request_parser_.parse(raw_packet, buffer_.data(), buffer_.data()+bytes_transferred);
            if (result == good) {
                std::unique_ptr<boost::thread>(new boost::thread(boost::bind(&connection::process_request, this)));
            } else {
                read();
            }
        } else {
            connection_manager_.stop(self);
        }
    });
}

void connection::stop() {
    boost::system::error_code ignored_ec;
    try {
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << e.what() << "\n";
    }

}

void connection::write() {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(raw_response),
    [this, self](boost::system::error_code ec, std::size_t){
        if (!ec) {
            BOOST_LOG_TRIVIAL(info) << "Successfully send response back to " << socket_.remote_endpoint().address().to_string() << "\n";
            BOOST_LOG_TRIVIAL(debug) << "Response: \n";
            BOOST_LOG_TRIVIAL(debug) << "---------Start---------: \n";
            BOOST_LOG_TRIVIAL(debug) << raw_response;
            BOOST_LOG_TRIVIAL(debug) << "----------End----------: \n";
            raw_response = "";
            connection_manager_.stop(self);
            BOOST_LOG_TRIVIAL(info) << "Shutdown socket " << socket_.remote_endpoint().address().to_string() << "\n";
            return;
        }
        BOOST_LOG_TRIVIAL(error) << "Send response failed \n";
    });
}

RequestHandler* connection::router(std::string url) {
    std::vector<std::string> uris;
    for (auto p = handler_mapping_.begin(); p != handler_mapping_.end(); p++) {
        uris.push_back(p->first);
    }
    std::string longest_match = longest_prefix_matching(uris, url);
    if (longest_match.length() != 0) {
        BOOST_LOG_TRIVIAL(info) << "Longest prefix matching for " << url << " is " << longest_match << "\n";
        return handler_mapping_[longest_match];
    }
    BOOST_LOG_TRIVIAL(info) << "Longest prefix matching for " << url << " Not Found\n";
    return handler_mapping_[NOT_FOUND_HANDLER];
}

std::string connection::longest_prefix_matching(std::vector<std::string>& uris, std::string& uri) {
    size_t max = 0;
    std::string res = "";
    for (std::string s: uris) {
        size_t n = uri.find(s);
        if (n == 0) {
            if (s == "/" || uri.length() == s.length() || uri[s.length()] == '/') {
                if (s.length() > max) {
                    max = s.length();
                    res = s;
                }
            }
        }
    }
    // If we haven't found a match and one of the URI prefixes is
    // the root directory, set it to that.
    if (res == "") {
        for (std::string prefix : uris) {
            if (prefix == "/") {
                res = prefix;
            }
        }
    }
    return res;
}

void connection::process_request() {
    BOOST_LOG_TRIVIAL(info) << "Receive HTTP Packet from: " << socket_.remote_endpoint().address().to_string() << "\n";
    BOOST_LOG_TRIVIAL(debug) << raw_packet << "\n";
    request_ = std::move(Request::Parse(raw_packet));
    RequestHandler::Status status = router(request_->uri())->HandleRequest(*request_.get(), &response_);
    if (status != RequestHandler::OK) {
        handler_mapping_[NOT_FOUND_HANDLER]->HandleRequest(*request_.get(), &response_);
    }
    StatusHandler::LogRequest(request_->uri(), response_.GetResponseCode());
    BOOST_LOG_TRIVIAL(info) << "Log Into StatusHandler uri: " << request_->uri() \
                        << " response code: " <<  response_.GetResponseCode() << "\n";
    raw_response = response_.ToString();
    write();
};
