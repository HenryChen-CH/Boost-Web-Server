//
// Created by 陈昊 on 2/4/17.
//

#include "request_handler_file.h"

request_handler_file::request_handler_file(std::string root_url, std::string dir_path) {
    this->dir_path = dir_path;
    this->root_url = root_url;
}

void request_handler_file::handle_request(const request &req, response &resp) {
    // replace root_path with actual dir path
    if (req.url().size() <= root_url.size()) {
        resp.status = bad_request;
        return;
    }
    std::string file_path = dir_path+req.url().substr(root_url.size());
    std::ifstream file(file_path, std::ios::binary);

    if (!file.good()) {
        resp.status = not_found;
        return;
    }

    resp.status = ok;
    std::string file_name = file_name_from_path(file_path);
    header h;
    h.name = "Content-Type";
    h.value = mime_types::extension_to_type(extension(file_name));
    resp.headers.push_back(h);

    resp.content.assign(std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());

}

std::string request_handler_file::file_name_from_path(const std::string file_path) {
    std::string file_name = "";
    for (unsigned int j = file_path.size()-1; j >= 0; j --) {
        if (file_path[j] == '/') break;
        file_name.insert(0, 1, file_path[j]);
    }
    return file_name;
}

std::string request_handler_file::extension(std::string file) {
    size_t n = file.rfind('.');
    if (n == std::string::npos) {
        return "";
    }
    return file.substr(n);
}
