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

        std::string _path;
        std::string _type;
        std::map<std::string, std::string> _header;
        std::string _msg;
        Request _req;
        Config _config;
        
        bool _is_autoindex;

        Response(Request &req, Config &config);

        std::string make_allow_error();
        void set_path();
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