#include "StaticFileHandler.h"

const std::string PathToken_ = "root";

RequestHandler::Status StaticFileHandler::HandleRequest(const Request& request,
                     Response* response) {
    try {
        std::string file_path = root_+request.uri().substr(uri_prefix_.length());
        std::ifstream file(file_path, std::ios::binary);

        if (!file.good()) {
            BOOST_LOG_TRIVIAL(error) << "StaticFileHandler file not found: " << file_path << "\n";
            return NOT_FOUND;
        }
        BOOST_LOG_TRIVIAL(info) << "StaticFileHandler is serving file: " << file_path << "\n";
        response->SetVersion("HTTP/1.1");
        response->SetStatus(Response::ok);
        std::string file_name = file_name_from_path(file_path);
        if (extension(file_name) == "md") {
            response->AddHeader(header_content_type_, "text/html");
            response->SetBody(markdown_to_html(file_path));
        } else {
            response->AddHeader(header_content_type_, mime_types::extension_to_type(extension(file_name)));
            std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            response->SetBody(body);
        }
        return OK;
    } catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "static file handler fail. " << e.what() << "\n";
        return NOT_FOUND;
    }
}

RequestHandler::Status StaticFileHandler::Init(const std::string& uri_prefix,
            const NginxConfig& config) {
    uri_prefix_ = uri_prefix;
    for (auto const& statement: config.statements_) {
        if (statement->tokens_[0] == PathToken_) {
            root_ = statement->tokens_[1];
            BOOST_LOG_TRIVIAL(info) << "Init StaticFileHandler " << "path: " << statement->tokens_[1] << "\n";
            return OK;
        }
    }
    BOOST_LOG_TRIVIAL(error) << "Init StaticFileHandler: path not found\n";
    return NOT_ENOUGH_PARAM;
}

std::string StaticFileHandler::file_name_from_path(const std::string file_path) const {
    std::string file_name = "";
    for (unsigned int j = file_path.size()-1; j >= 0; j --) {
        if (file_path[j] == '/') break;
        file_name.insert(0, 1, file_path[j]);
    }
    return file_name;
}

std::string StaticFileHandler::extension(std::string file) const {
    size_t n = file.rfind('.');
    if (n == std::string::npos) {
        return "";
    }
    return file.substr(n+1);
}

std::string StaticFileHandler::markdown_to_html(std::string md_file) {
    markdown::Document doc;
    if (doc.read(md_file)) {
        std::ostringstream md;
        doc.write(md);
        return md.str();
    } else {
        return "";
    }
}
