#include "request_parser.hpp"

request_parser::request_parser() {

}

result_type request_parser::parse(request& req, char* start, char* end) {
    while (start != end) {
        result_type  result = consume(req, *start);
        start++;
        if (result != intermedia) {
            return result;
        }
    }
    return intermedia;
}

result_type request_parser::consume(request &req, char input) {
    req.header.push_back(input);
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
