#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "header.hpp"
#include <iostream>

enum reponse_status {
    ok = 200,
    bad_request = 400,
    not_found = 404
};

class response {
public:
    response();

    std::string ToString();
    std::vector<boost::asio::const_buffer> ToBuffer();

    reponse_status  status;
    std::vector<header> headers;
    std::vector<char> content;

private:
    std::string to_string(reponse_status status);
    boost::asio::const_buffer to_buffer(reponse_status status);
};

#endif
