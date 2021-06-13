#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "lib.hpp"
#include "config.hpp"
#include <string>
#include <map>

class Location {
    
};

class Request {
    public:
        std::string _raw;
        std::string _Method;
        std::string _Uri;
        std::string _protocol;
        std::map<std::string, std::string> _header;
        std::string _body;

        bool _ready;
        Request(std::string &req, Config &config);

        void parse();
        void parse_request_line();
        void parse_header();
        void get_body();
};

#endif

/*
    //Common//
    Method;
    Date;
    Connection;

    //Entity//
    Content-Type;
    Content-Language;
    Content-Length;
    Content-Location;
    
    //Request//
    Accept-Charsets;
    Accept-Language;
    Authorization;
    Host;
    Referer;
    User-Agent;
*/
