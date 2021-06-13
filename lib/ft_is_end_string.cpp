#include "lib.hpp"

bool ft_is_end_string(std::string str, std::string end) {
    std::size_t i = str.size();
    std::size_t j = end.size();

    while (j > 0) {
        i--;
        j--;
        if (i < 0 || str[i] != end[j])
            return false;
    }
    return true;
}
