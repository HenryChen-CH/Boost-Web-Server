#include "http_server.h"

http_server::http_server()
: io_service_(),
  acceptor_(io_service_),
  socket_(io_service_),
  signals_(io_service_)
{
    signals_.add(SIGINT);
    stop();
}

bool http_server::Init(NginxConfig& config)
{
    std::vector<std::shared_ptr<NginxConfigStatement>> statements = config.statements_;
    for (auto const& statement: statements) {
        if (statement.get()->tokens_[0] == "listen") {
            port = statement.get()->tokens_[1];
            BOOST_LOG_TRIVIAL(info) << "Server is listening on port: " << port << "\n";
        } else if (statement.get()->tokens_[0] == "path") {
            RequestHandler * tmp = RequestHandler::CreateByName(statement->tokens_[2].c_str());
            if (tmp == nullptr) {
                BOOST_LOG_TRIVIAL(error) << "Handler: " <<  statement->tokens_[2].c_str() << " Not Found\n";
                return false;
            }
            if (handler_mapping_.find(statement->tokens_[1]) != handler_mapping_.end()) {
                BOOST_LOG_TRIVIAL(error) << "Duplicate Path: " << statement->tokens_[1] << "\n";
                return false;
            }
            StatusHandler::LogHandler(statement->tokens_[1], statement->tokens_[2]);
            RequestHandler::Status status = tmp->Init(statement->tokens_[1], *statement->child_block_.get());
            if (status != RequestHandler::OK) {
                return false;
            }
            BOOST_LOG_TRIVIAL(info) << statement->tokens_[1] << " " << statement->tokens_[2] << "\n";
            handler_mapping_[statement->tokens_[1]] = tmp;
        }
    }
    RequestHandler* tmp = RequestHandler::CreateByName("NotFoundHandler");
    if (tmp == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "NotFoundHandler class not found\n";
        return false;
    }
    StatusHandler::LogHandler(NOT_FOUND_HANDLER, "NotFoundHandler");
    handler_mapping_[NOT_FOUND_HANDLER] = tmp;

    try {
        int port_ = std::stoi(port);
        boost::asio::ip::tcp::endpoint endpoint_(boost::asio::ip::tcp::v4(), port_);
        acceptor_.open(endpoint_.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint_);
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
            BOOST_LOG_TRIVIAL(error) << "Acceptor is closed\n";
            return;
        }
        if (!ec) {
            BOOST_LOG_TRIVIAL(info) << "Accept connection from: " << socket_.remote_endpoint().address().to_string() << "\n";
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
        BOOST_LOG_TRIVIAL(info) << "Server stop\n";
        exit(0);
    });
}
