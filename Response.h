#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>

class Response {
public:
    enum ResponseCode {
        // Define your HTTP response codes here.
    };

    void SetStatus(const ResponseCode response_code);
    void AddHeader(const std::string& header_name, const std::string& header_value);
    void SetBody(const std::string& body);

    std::string ToString();
};
#endif
