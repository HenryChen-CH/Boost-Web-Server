#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "request.h"
#include "response.h"
#include <boost/asio.hpp>
#include "header.h"
#include <string>


class request_handler{
public:
    virtual void handle_request(const request& req, response& resp){resp.status = not_found;};
private:
};

#endif
