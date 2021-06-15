#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "lib.hpp"
#include <string>
#include <map>
#include "request.hpp"
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

class Response {
    public:
        int _code;
        std::string _host;
        int _port;

        std::map<std::string, Location> _locations;
        std::map<int, std::string> _error_page;
        std::vector<std::string> _methods;

        std::string _path;
        std::string _type;
        std::map<std::string, std::string> _header;
        std::string _msg;
        Request _req;
        
        bool _is_autoindex;

        Response(
            Request &req,
            std::map<std::string, Location> locations
            );
        void set_location();
        void set_param(Location location, std::string index);
        std::string make_allow_error();
        
        void GET();
        void HEAD();
        void POST();
        void PUT();
        void DELETE();
        void OPTIONS();
        void TRACE();

        int read_data();
        int write_data(std::string data);
        std::string make_header();
        std::string set_type();
        std::string set_redirect();
        std::string set_retry();
        std::string set_auth();
        std::string set_allow();
        
        std::string to_string();
        std::string load_html(std::string path);
};

#endif

/*
    _allow = "";
	_contentLanguage = "";
	_contentLength = "";
	_contentLocation = "";
	_contentType = "";
	_date = "";
	_lastModified = "";
	_location = "";
	_retryAfter = "";
	_server = "";
	_transferEncoding = "";
	_wwwAuthenticate = "";
*/