#include "request.h"

request::request() {

}

std::string request::ToString() {
    std::string package;

    for (char c: header) {
        package += c;
    }
    for (char c: body) {
        package += c;
    }
    std::cout << "Pack: " << package << std::endl;
    return package;
}

std::string request::url() const{
    std::string result = "";
    unsigned int i = 0;

    for (i = 0; i < header.size(); i++) {
        if (header[i] == ' ') break;
    }
    for (unsigned int j = i+1; j < header.size(); j++) {
        if (header[j] == ' ') break;
        result.push_back(header[j]);
    }

    return result;
}
