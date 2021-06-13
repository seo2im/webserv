#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "lib.hpp"
#include <string>
#include <map>
#include "request.hpp"
#include "config.hpp"

class Response {
    public:
        std::string _Method;
        std::string _Uri;
        std::string _protocol;
        std::map<std::string, std::string> _header;
        std::string _body;

        Response(Request &req, Config &config);

        std::string to_string();
};

#endif