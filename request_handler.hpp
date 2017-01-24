#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "request.hpp"
#include "response.hpp"
#include <boost/asio.hpp>
#include "request.hpp"


class request_handler{
public:
    request_handler();
    void handle_request(const request& req, response& reponse);
private:

};

#endif