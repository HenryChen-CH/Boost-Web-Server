#include "ProxyHandler.h"

const std::string HOST_TOKEN = "host";
const std::string PORT_TOKEN = "port";

using boost::asio::ip::tcp;

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request,
                     Response* response) {

    // Get endpoints corresponding to the server name.
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host_, "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    // Get requested path.
    std::string file_path = request.uri().substr(uri_prefix_.length());

    // Form the request.
    boost::asio::streambuf proxy_request;
    std::ostream request_stream(&proxy_request);
    request_stream << "GET / HTTP/1.1\r\n";
    request_stream << "Host: " << host_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: keep-alive\r\n\r\n";

    // Send the request.
    boost::asio::write(socket, proxy_request);
    BOOST_LOG_TRIVIAL(info) << "Sending proxy HTTP request:\n" << &proxy_request << "\n\n";

    // Read the response.
    boost::asio::streambuf proxy_response;
    boost::asio::read_until(socket, proxy_response, "\r\n");
    std::istream response_stream(&proxy_response);
    std::ostringstream response_output;

    // Check the response.
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
        BOOST_LOG_TRIVIAL(error) << "Invalid HTTP response\n";
        return RequestHandler::PROXY_FAILURE;
    }
    // TODO: handle HTTP redirect
    if (status_code != 200) {
        BOOST_LOG_TRIVIAL(info) << "Response returned with status code " << status_code << "\n";
    }

    // Read response headers.
    boost::asio::read_until(socket, proxy_response, "\r\n\r\n");

    // Process response headers.
    std::string header;
    std::size_t colon_loc;
    while (std::getline(response_stream, header) && header != "\r") {
        colon_loc = header.find(":");
        response->AddHeader(header.substr(0, colon_loc), header.substr(colon_loc+1));
        BOOST_LOG_TRIVIAL(info) << header << "\n";
    }

    // Write whatever content we already have to output.
    if (proxy_response.size() > 0) {
        response_output << &proxy_response;
    }

    // Read until EOF, writing data to output as we go.
    boost::system::error_code error;
    while (boost::asio::read(socket, proxy_response,
        boost::asio::transfer_at_least(1), error)) {
        response_output << &proxy_response;
    }
    if (error != boost::asio::error::eof) {
        throw boost::system::system_error(error);
    }

    // Fill out response.
    response->SetStatus(Response::ok);
    response->SetVersion(http_version);
    response->SetBody(response_output.str());

    return RequestHandler::OK;
}

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    uri_prefix_ = uri_prefix;

    // Get remote host and post from config.
    if (config.statements_.size() >= 2) {
        for (auto const& statement: config.statements_) {
            std::vector<std::string> tokens = statement->tokens_;
            if (tokens.size() == 2) {
                if (tokens.at(0) == HOST_TOKEN) {
                    host_ = tokens.at(1);
                }
                else if (tokens.at(0) == PORT_TOKEN) {
                    port_ = tokens.at(1);
                }
            }
        }

        // Error checking on config parameters.
        if (host_ == "") {
            BOOST_LOG_TRIVIAL(error) << "No host specified\n";
            return BAD_CONFIG;
        }
        int port_as_int;
        try {
            port_as_int = std::stoi(port_);
        }
        catch (std::exception const &e) {
            BOOST_LOG_TRIVIAL(error) << "Port number is not a valid number\n";
            return BAD_CONFIG;
        }
        if (port_as_int > 65535 || port_as_int < 0) {
            BOOST_LOG_TRIVIAL(error) << "Port is outside acceptable range\n";
            return BAD_CONFIG;
        }

        BOOST_LOG_TRIVIAL(info) << "Init ProxyHandler " << "host: " << host_ << ", port: " << port_ << "\n";
        return OK;
    }
    BOOST_LOG_TRIVIAL(error) << "Init ProxyHandler: bad config\n";
    return NOT_ENOUGH_PARAM;
}