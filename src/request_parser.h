#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <string>

enum result_type { good, bad, intermedia };

class request_parser{
public:
    request_parser();
    result_type parse(std::string& packet, char* start, char* end);
private:

    result_type consume(std::string& req, char input);
    enum state{
        header_start,
        header_normal,
        header_end1,
        header_end2,
        header_end3,
    };

    state state_ = header_start;

};

#endif
