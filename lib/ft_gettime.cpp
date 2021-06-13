#include "lib.hpp"

std::string ft_gettime() {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = gmtime(&tv.tv_sec);

    char buffer[1000];
    strftime(buffer, 1000, "%a, %d %b %Y %H:%M:%S GMT", tm);
    return std::string(buffer);
}
