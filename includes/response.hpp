#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "lib.hpp"
#include <string>
#include <map>
#include <algorithm>
#include "request.hpp"
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "cgi.hpp"

class Response {
    public:
        int _code;
        long _host;
        int _port;

        std::map<std::string, Location> _locations;
        std::map<int, std::string> _error_page;
        std::vector<std::string> _methods;
        size_t _buffer_size;
        std::string _index;
        std::string _cgi;
        std::string _name;

        std::string _path;
        std::string _type;
        std::map<std::string, std::string> _header;
        std::string _msg;
        Request _req;
        
        bool _is_autoindex;

        Response(
            Request &req,
            std::map<std::string, Location> locations,
            long host,
            int port,
            std::string name
            );
        void set_location();
        void set_param(Location location, std::string index);
        void set_allow_params(Location &location);
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
        std::string set_last_modified();
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