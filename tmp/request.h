#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <boost/asio.hpp>
#include <vector>
#include <iostream>
#include <string>

class request{
public:
    request();
    std::vector<char> header;
    std::vector<char> body;

    std::string ToString();

    std::string url() const;
private:

};

#endif
