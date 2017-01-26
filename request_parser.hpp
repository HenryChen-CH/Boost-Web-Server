#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "request.hpp"

enum result_type { good, bad, intermedia };

class request_parser{
public:
    request_parser();
    result_type parse(request& req, char* start, char* end);
private:

    result_type consume(request& req, char input);
    enum state{
        header_start,
        header_normal,
        header_end1,
        header_end2,
        header_end3,
        body_start
    };

    state state_ = header_start;

};

#endif