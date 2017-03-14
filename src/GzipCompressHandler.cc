#include "GzipCompressHandler.h"

RequestHandler::Status GzipCompressHandler::HandleRequest(const Request& request, Response* response) {
    if (support_gzip(request)) {
        // set encode type header
        response->AddHeader("Content-Encoding", enocode_type_);

        // compress body
        BOOST_LOG_TRIVIAL(info) << "body origin size: " << response->GetBody().length();
        std::string compressed = compress(response->GetBody());
        response->SetBody(compressed);
        BOOST_LOG_TRIVIAL(info) << "body compressed size: " << response->GetBody().length();
    } else {
        BOOST_LOG_TRIVIAL(info) << "browser does not support gzip compress";
    }
    return OK;
}


/* do not need initializatin */

RequestHandler::Status GzipCompressHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return OK;
}


/* whether browser support gzip */

bool GzipCompressHandler::support_gzip(const Request& request) {
    std::string accept_encode = request.GetHeader("Accept-Encoding");
    if (accept_encode.find("gzip") != std::string::npos) {
        return true;
    }
    return false;
}

/* compress */

std::string GzipCompressHandler::compress(const std::string& data) {
    namespace bio = boost::iostreams;

    std::stringstream compressed;
    std::stringstream origin(data);

    bio::filtering_streambuf<bio::input> out;
    out.push(bio::gzip_compressor(bio::gzip_params(bio::gzip::best_compression)));
    out.push(origin);
    bio::copy(out, compressed);

    return compressed.str();
}
