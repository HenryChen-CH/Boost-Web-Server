#include "http_server.hpp"

http_server::http_server()
: io_service_(),
  acceptor_(io_service_),
  socket_(io_service_),
  signals_(io_service_)
{
    signals_.add(SIGINT);
    stop();
}

bool http_server::Init(const std::string& config_file)
{
    NginxConfigParser parser_;
    if (!parser_.Parse(config_file.data(), &config_)) {
        return false;
    }

    std::vector<std::shared_ptr<NginxConfigStatement>> statements = config_.statements_[0].get()->child_block_.get()->statements_;//[0].get()->tokens_;
    for (auto const& statement: statements) {
        if (statement.get()->tokens_[0] == "listen") {
            port = statement.get()->tokens_[1];
        } else if (statement.get()->tokens_[0] == "route") {
            route_block_parse(statement.get()->child_block_.get());
        }
    }
    handler_mapping_["/"] = new request_handler();
    
    try {
        boost::asio::ip::tcp::resolver resolver(io_service_);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({"localhost", port});
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        accept();
    } catch (const boost::system::system_error& error) {
        std::cout << error.what() << std::endl;
        return false;
    }

    return true;
}

void http_server::accept() {
    acceptor_.async_accept(socket_,
    [this](boost::system::error_code ec) {
        if (!acceptor_.is_open()) {
            return;
        }
        if (!ec) {
            connection_manager_.start(std::make_shared<connection>(std::move(socket_), connection_manager_, handler_mapping_));
        }
        accept();
    });
}

void http_server::start() {
    io_service_.run();
}

void http_server::stop() {
    signals_.async_wait([this](boost::system::error_code ec, int signo){
        connection_manager_.stop_all();
        std::cout << "Server Stop\n";
        exit(0);
    });
}

void http_server::route_block_parse(NginxConfig *config) {
    std::string root_url, dir;
    for (auto const& statement: config->statements_) {
        if (statement.get()->tokens_[0] == "root_url") {
            root_url = statement.get()->tokens_[1];
        } else if (statement.get()->tokens_[0] == "dir") {
            dir = statement.get()->tokens_[1];
        }
    }
    if (root_url == "/echo") handler_mapping_[root_url] = new request_handler_echo();
    else {
        handler_mapping_[root_url] = new request_handler_file(root_url, dir);
    }
}
