// include/order_types.hpp
#ifndef ORDER_TYPES_HPP
#define ORDER_TYPES_HPP

#include <string>

struct MatchResult {
    double price{0.0};
    int quantity{0};
    std::string status;

    MatchResult() = default;
    MatchResult(double p, int q, std::string s) 
        : price(p), quantity(q), status(std::move(s)) {}
};

#endif // ORDER_TYPES_HPP
