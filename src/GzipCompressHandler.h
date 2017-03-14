#ifndef COMPRESSHANDLER_H
#define COMPRESSHANDLER_H
#define BOOST_IOSTREAMS_NO_LIB
#include "RequestHandler.h"
#include "nginx-configparser/config_parser.h"
#include <boost/log/trivial.hpp>
#include <string>
#include <sstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

class GzipCompressHandler : public RequestHandler {
public:
    Status Init(const std::string& uri_prefix, const NginxConfig& config);
    Status HandleRequest(const Request& request, Response* response);

private:
    std::string enocode_type_ = "gzip";
    bool support_gzip(const Request& request);
    std::string compress(const std::string& data);
};

REGISTER_REQUEST_HANDLER(GzipCompressHandler);

#endif
