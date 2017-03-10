#include "connection.h"
#include "connection_manager.h"


const std::string content_length_ = "Content-Length";
const std::string connection_ = "Connection";
const std::string keep_alive_ = "keep-alive";

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
    boost::asio::async_read_until(socket_, bufstream_, "\r\n\r\n", [this, self](boost::system::error_code ec, std::size_t bytes_transferred){
        if (!ec) {
            std::string buffer( (std::istreambuf_iterator<char>(&bufstream_)), std::istreambuf_iterator<char>());
            bufstream_.consume(bufstream_.size());
            request_ = std::move(Request::Parse(buffer));
            read_body();
        } else {
            connection_manager_.stop(self);
            return;
        }
    });
}

void connection::read_body() {
    auto self(shared_from_this());
    if (request_->GetHeader(content_length_).length() != 0) {
        int length = std::stoi(request_->GetHeader(content_length_));
        if (request_->body().length() != length) {
            socket_.async_read_some(boost::asio::buffer(buffer_), [this, self](boost::system::error_code ec, std::size_t bytes_transferred){
                if (!ec) {
                    std::string buffer;
                    std::copy(buffer_.begin(), buffer_.begin()+bytes_transferred, std::back_inserter(buffer));
                    request_->AppendBody(buffer);
                    read_body();
                    return;
                } else {
                    connection_manager_.stop(self);
                    return;
                }
            });
            return;
        }
    }
    std::unique_ptr<boost::thread>(new boost::thread(boost::bind(&connection::process_request, this)));
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
            BOOST_LOG_TRIVIAL(debug) << "Connection: " << request_->GetHeader(connection_) << "\n";
            if (request_->GetHeader(connection_) != keep_alive_) {
                connection_manager_.stop(self);
                return;
            }
            read();
        } else {
            BOOST_LOG_TRIVIAL(error) << "Send response failed \n";
            connection_manager_.stop(self);
        }
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
    BOOST_LOG_TRIVIAL(debug) << request_->raw_request() << "\n";
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
