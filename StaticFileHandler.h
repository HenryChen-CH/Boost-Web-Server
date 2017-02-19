#ifndef STATICFILEHANDLER_H
#define STATICFILEHANDLER_H

#include "RequestHandler.h"
#include "nginx-configparser/config_parser.h"
#include "mime_types.h"
#include <fstream>
#include <streambuf>
#include <boost/log/trivial.hpp>

class StaticFileHandler : public RequestHandler{
public:
    Status HandleRequest(const Request& request, Response* response);
    Status Init(const std::string& uri_prefix, const NginxConfig& config);

private:
    std::string uri_prefix_;
    std::string root_;

    std::string file_name_from_path(std::string file_path) const;
    std::string extension(std::string file) const;
};

REGISTER_REQUEST_HANDLER(StaticFileHandler);

#endif
