#include "ProxyHandler.h"
#include <iostream>

const std::string HOST_TOKEN = "host";
const std::string PORT_TOKEN = "port";

using boost::asio::ip::tcp;

RequestHandler::Status ProxyHandler::HandleRequest(const Request& request,
                     Response* response) {

    Request r_request = request;
    //Make initial request to server
    std::string response_string = NewRequest(r_request, response);
    ParseResponse(response_string, response);

    return RequestHandler::OK;
}

std::string ProxyHandler::NewRequest(const Request& request,
                     Response* response) {

    // Get endpoints corresponding to the server name.
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host_, port_);
    boost::system::error_code error;
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, error), end;
    if (error) {
        BOOST_LOG_TRIVIAL(error) << "Unable to resolve host " << host_ << "and port " << port_ << "\n";
        return "failure";
    }

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator, end, error);
    if (error) {
        BOOST_LOG_TRIVIAL(error) << "Unable to bind socket. Host: " << host_ << "\n";
        return "failure";
    }

    // Get request string and send it.
    BOOST_LOG_TRIVIAL(info) << "Requested URI is " << request.uri() << "\n\n";
    std::string request_string = GetRequestString(request.uri());
    boost::asio::streambuf proxy_request;
    std::ostream request_stream(&proxy_request);
    request_stream << request_string;
    boost::asio::write(socket, proxy_request);
    BOOST_LOG_TRIVIAL(info) << "Sending proxy HTTP request:\n" << request_string << "\n\n";

    // Read the response.
    std::string response_string;
    boost::asio::streambuf proxy_response;
    std::size_t bytes_read;
    while ((bytes_read = boost::asio::read(socket, proxy_response,
        boost::asio::transfer_at_least(1), error))) {
        std::string new_data = std::string(boost::asio::buffers_begin(proxy_response.data()),
            boost::asio::buffers_begin(proxy_response.data()) + bytes_read);
        response_string += new_data;
        std::cout << new_data;

        proxy_response.consume(bytes_read);
    }
    if (error != boost::asio::error::eof) {
        BOOST_LOG_TRIVIAL(error) << "Error reading HTTP response from server.\n";
        return "failure";
    }

    std::cout << response_string;

    return response_string;
}

std::string ProxyHandler::GetRequestString(const std::string& uri) {
    std::string request_string = "";
    // Get requested path.
    std::string file_path = "/";
    if (uri.length() > uri_prefix_.length()) {
        file_path = uri.substr(uri_prefix_.length());
    }
    // Form the request.
    request_string += "GET " + file_path + " HTTP/1.1\r\n";
    request_string += "Host: " + host_ + ":" + port_ + "\r\n";
    request_string += "Accept: */*\r\n";
    request_string += "Connection: keep-alive\r\n\r\n";
    return request_string;
}

Response::ResponseCode ProxyHandler::ParseResponse(std::string response_string, Response* response) {
    int index;
    std::string header;
    std::string body;
    std::string first_line;
    std::string headers;
    std::string http_version = "";
    std::string status_code = "";

    // Split response into header and body section
    index = response_string.find("\r\n\r\n");

    if (index == std::string::npos || index + 4 > response_string.size()) {
        BOOST_LOG_TRIVIAL(error) << "Couldn't find end of header section in HTTP response.\n";
        return Response::ResponseCode::bad_request;
    }

    header = response_string.substr(0, index);
    BOOST_LOG_TRIVIAL(info) << "Header: \n" << header << "\n";
    body = response_string.substr(index + 4);

    // Split header section into first line and headers
    index = header.find("\r\n");
    if (index == std::string::npos || index + 2 > header.size()) {
        BOOST_LOG_TRIVIAL(error) << "HTTP header poorly formed: " << header << "\n";
        return Response::ResponseCode::bad_request;
    }

    first_line = header.substr(0, index);
    headers = header.substr(index + 2);

    // Parse first line
    index = first_line.find(" ");
    http_version = first_line.substr(0, index);
    BOOST_LOG_TRIVIAL(info) << "HTTP version: " << http_version << "\n";
    first_line = first_line.substr(index + 1);
    index = first_line.find(" ");
    status_code = first_line.substr(0, index);
    BOOST_LOG_TRIVIAL(info) << "Status code: " << status_code << "\n";

    // Headers
    std::string name;
    std::string value;
    std::string header_line;
    int colon_index;
    int line_end;
    index = headers.find("\r\n");
    while (index != std::string::npos && index < headers.size()) {
        line_end = headers.find("\r\n");
        header_line = headers.substr(0, line_end);
        std::cout << header_line;
        colon_index = header_line.find(":");
        name = header_line.substr(0, colon_index);
        value = header_line.substr(colon_index + 2);
        response->AddHeader(name, value);
        BOOST_LOG_TRIVIAL(info) << "HEADER " << name << ": " << value << "\n";
        headers = headers.substr(index + 2);
        index = headers.find("\r\n");
    }

    response->SetStatus(GetStatusCode(status_code));
    response->SetVersion(http_version);
    response->SetBody(body);

    return Response::ResponseCode::found;
}

Response::ResponseCode ProxyHandler::GetStatusCode(const std::string& status_code_string) {
    int status_code_int = stoi(status_code_string);
    switch (status_code_int) {
        case 200:
            return Response::ok;
        case 302:
            return Response::found;
        case 400:
            return Response::bad_request;
        default:
            return Response::not_found;
    }
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