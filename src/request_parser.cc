#include "request_parser.h"

request_parser::request_parser() {

}

result_type request_parser::parse(std::string& req, char* start, char* end) {
    while (start != end) {
        result_type  result = consume(req, *start);
        start++;
        if (result != intermedia) {
            return result;
        }
    }
    return intermedia;
}

result_type request_parser::consume(std::string& req, char input) {
    req.push_back(input);
    switch(input) {
        case '\r':
            switch (state_) {
                case header_normal:
                    state_ = header_end1;
                    return intermedia;
                case header_end2:
                    state_ = header_end3;
                    return intermedia;
                default:
                    return bad;
            }
        case '\n':
            switch(state_) {
                case header_end1:
                    state_ = header_end2;
                    return intermedia;
                case header_end3:
                    state_ = header_start;
                    return good;
                default:
                    return bad;
            }
        default:
            state_ = header_normal;
            return intermedia;
    }
}
