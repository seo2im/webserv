#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "lib.hpp"
#include "config.hpp"
#include <string>
#include <map>

// class Location {
    
// };

class Request {
    public:
        std::string _raw;

        std::string _method;
        std::string _uri;
        std::string _protocol;
        std::map<std::string, std::string> _header;
        std::string _body;
        int _code;
        int _port;
        std::map<std::string, std::string> _cgi;
        
        Request();
        Request(std::string &raw);
        Request & operator=(const Request & src);

        std::string chunked_process(std::string &raw);
        void init();
        void parse();
        std::string parse_request_line(std::string raw);
        std::string parse_header(std::string raw);
        std::string parse_body(std::string raw);
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
