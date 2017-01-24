#include "request.hpp"

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