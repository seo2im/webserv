#include "lib.hpp"

std::string ft_num2string(size_t n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}