#include "response.hpp"

Response::Response(
        Request &req,
        std::map<std::string, Location> locations,
        long host,
        int port
    ) {

    _req = req;
    _code = _req._code;
    _host = host;
    _port = port;
    _locations = locations;
    set_location();

    if (std::find(_methods.begin(), _methods.end(), _req._method) == _methods.end()) {
        _code = 405;
    } else if (_buffer_size < _req._body.size() - 4) _code = 413; //TODO: test code of cgi size (have to check buffer size)

    
    // std::cout << "this is sparta" << std::endl;
    // std::cout << "size" <<  _buffer_size <<std::endl;
    // std::cout << "real" << _req._body.size() <<  std::endl;

    if (_code == 405 || _code == 413) {
        // std::cout << "WORK\n" << std::endl;
        _msg = make_allow_error();
        // std::cout << _msg << std::endl;
    } else {
        if (_req._method == "GET") {
            GET();
        } else if (_req._method == "HEAD") {
            HEAD();
        } else if (_req._method == "POST") {
            POST();
        } else if (_req._method == "PUT") {
            PUT();
        } else if (_req._method == "DELETE") {
            DELETE();
        } else if (_req._method == "OPTIONS") {
            OPTIONS();
        } else if (_req._method == "TRACE") {
            TRACE();
        }
    }

    
}

void Response::set_param(Location location, std::string index) {
    _path = location._root + index;
    _index = location._index;
    _error_page = location._error_page;
    _methods = location._methods;
    _buffer_size = location._buffer_size;    
    _cgi = location._cgi;
}

void Response::set_location() {
    size_t i;

    if ((i = _req._uri == "")) {
        set_param(_locations["/"], _locations["/"]._index);
        return ;
    }

    i = _req._uri.find_first_of('/');
    std::string first_path = _req._uri.substr(0, i);
    if (_locations.count(first_path) > 0) {
        if (i == std::string::npos) {
            set_param(_locations[first_path], _locations[first_path]._index);
        } else {
            set_param(_locations[first_path], _req._uri.substr(i + 1)); 
        }
    } else {
        set_param(_locations["/"], _req._uri);
    }

    if ((i = _req._uri.find_last_of('.')) != std::string::npos) {
        std::string format = _req._uri.substr(i);
        if (_locations.count(format) > 0) {
            set_allow_params(_locations[format]);
            return ;
        }
    }
}

void Response::set_allow_params(Location &location) {
    _cgi = location._cgi;
    for (std::vector<std::string>::iterator it = location._methods.begin(); it != location._methods.end(); it++) {
        _methods.push_back((*it));
    }
}

void Response::GET() {
    if (_cgi != "") {
        Cgi cgi(_req, _path, _host, _port);
        size_t i = 0;
        size_t j = _msg.size() - 2;

        _msg = cgi.executeCgi(_cgi);

        while (_msg.find("\r\n\r\n", i) != std::string::npos || _msg.find("\r\n", i) == i)
		{
			std::string	str = _msg.substr(i, _msg.find("\r\n", i) - i);
			if (str.find("Status: ") == 0)
				_code = std::atoi(str.substr(8, 3).c_str());
			else if (str.find("Content-Type: ") == 0)
				_type = str.substr(14, str.size());
			i += str.size() + 2;
		}
		while (_msg.find("\r\n", j) == j)
			j -= 2;

		_msg = _msg.substr(i, j - i);
    }
    else if (_code == 200) _code = read_data();
    else _msg = load_html(_error_page[_code]);

    /* TODO: check why it need */
    if (_code == 500) _msg = load_html(_error_page[500]);

    _msg = make_header() + "\r\n" + _msg;
}


int Response::read_data() {
    std::ifstream file;
    std::stringstream ss;
    int is_file;
    
    if ((is_file = ft_checkfile(_path.c_str())) == 1) {
        file.open(_path.c_str(), std::ifstream::in);
        if (!file.is_open()) {
            _msg = load_html(_error_page[403]);
            return 403;
        }

        ss << file.rdbuf();
        _msg = ss.str();

        file.close();
        return 200; /* TODO: check right type */
    }
    else if (is_file == 2) {
        if (ft_checkfile((_path + "/" + _index).c_str()) == 1) {
            file.open(_path.c_str(), std::ifstream::in);
            if (!file.is_open()) {
                _msg = load_html(_error_page[403]);
                return 403;
            }

            ss << file.rdbuf();
            _msg = ss.str();
            file.close();
            return 200; /* TODO: check right type */
        } else {
            _msg = load_html(_error_page[404]);
            return 404;
        }
    } else {
        _msg = load_html(_error_page[404]);
        return 404;
    }
}

std::string Response::make_header() {
    _header["Allow"] = "";
    _header["Content-Language"] = "lang"; /* TODO: set lang witch Accept header*/
    _header["Content-Location"] = _path;
	_header["Content-Length"] = ft_num2string(_msg.size());
	_header["Content-Type"] = set_type();
	_header["Date"] = ft_gettime();
	_header["Last-Modified"] = ft_gettime(); /* TODO: set file modified time? */
	_header["Location"] = set_redirect();
	_header["Retry-After"] = set_retry();
	_header["Server"] = "HTTP(Unix)"; /* TODO: Anything? */
	_header["Transfer-Encoding"] = "identity";
	_header["WWW-Authenticate"] = set_auth();

    std::string header_msg = "HTTP/1.1 " + ft_num2string(_code) \
            + " OK" + "\r\n";
    for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); it++) {
        if ((*it).second != "") {
            header_msg += ((*it).first + ": " + (*it).second + "\r\n");
        }
    }
    
    // std::cout << header_msg << std::endl; //TESTING CODE
    return header_msg;
}

std::string Response::set_type() {
    if (_type != "") return _type;
    _type = _path.substr(_path.rfind(".") + 1, _path.size() - _path.rfind("."));
    if (_type == "html") return "text/html";
	else if (_type == "css") return "text/css";
	else if (_type == "js") return "text/javascript";
	else if (_type == "jpeg" || _type == "jpg") return "image/jpeg";
	else if (_type == "png") return "image/png";
	else if (_type == "bmp") return "image/bmp";
	else return "text/plain";
}

std::string Response::set_redirect() {
    if (_code == 201 || _code / 100 == 3) return _path;
    return "";
}

std::string Response::set_retry() {
    if (_code == 503 || _code == 429 || _code == 301) return "3"; /* TODO: check 3s is standard */
    return "";
}

std::string Response::set_auth() {
    if (_code == 401) return "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
    return "";
}

std::string Response::set_allow() {
    std::string str;

    for (std::vector<std::string>::iterator it = _methods.begin(); it != _methods.end(); it++) {
        str += *(it);

        if (it != _methods.end())
            str += ", ";
    }

    return str;
}

std::string Response::to_string() {
    return _msg;
}

std::string Response::load_html(std::string path) {
    std::ofstream		file;
	std::stringstream	buffer;

	if (ft_checkfile(path.c_str()) == 1)
	{
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");

		buffer << file.rdbuf();
		file.close();
		_type = "text/html";

		return (buffer.str());
	}
	else
		return ("<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n");
}

std::string Response::make_allow_error() {
    
    _header["Allow"] = set_allow();
    _header["Content-Language"] = "lang"; /* TODO: set lang witch Accept header*/
    _header["Content-Location"] = "Content-location" /* TODO: config content location make something */;
	_header["Content-Length"] = ft_num2string(0);
    
    _type = "";
	_header["Content-Type"] = set_type();
	
    _header["Date"] = ft_gettime();
	_header["Last-Modified"] = ft_gettime(); /* TODO: set file modified time? */
	_header["Location"] = set_redirect();
	_header["Retry-After"] = set_retry();
	_header["Server"] = "HTTP(Unix)"; /* TODO: Anything? */
    
	_header["Transfer-Encoding"] = "identity";
	_header["WWW-Authenticate"] = set_auth();

    std::string header_msg = "HTTP/1.1 ";

    if (_code == 405) header_msg += "405 Method Not Allowed\r\n";
    else if (_code == 413) header_msg += "413 Payload Too Large\r\n";
    else header_msg += (ft_num2string(_code) \
            + /* if error insert error in herer*/ " \r\n");
    for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); it++) {
        if ((*it).second != "") {
            header_msg += ((*it).first + ": " + (*it).second + "\r\n");
        }
    }
    
    // std::cout << header_msg << std::endl; //TESTING CODE

    return header_msg + "\r\n";
}

void Response::HEAD() {
    _code = read_data();
    _msg = make_header() + "\r\n";
}
/*
    TODO: Check why it no parse of body...
*/
void Response::POST() {
    if (_cgi != "") {
        Cgi cgi(_req, _path, _host, _port);

        _msg = cgi.executeCgi(_cgi);
        
        size_t i = _msg.find("\r\n\r\n");
        size_t j = _msg.size();
        
        std::string	str = _msg.substr(0, i);
        if (str.find("Status: ") == 0)
		    _code = std::atoi(str.substr(8, 3).c_str());
        else if (str.find("Content-Type: ") == 0)
		    _type = str.substr(14, str.size());
        i = i + 4;
		j =_msg.substr(i + 1).find("\r\n\r\n");
        if (j == std::string::npos) {
            j = _msg.size() - i - 1;
        }
			
		_msg = _msg.substr(i + 4, i + j + 1);
        // std::cout << "i : " << i << std::endl;
        // std::cout << "j : " << j << std::endl;
        // std::cout << "body : " << _msg.size() << std::endl; //TESTING CODE
        // std::cout << "start : " <<_msg.substr(0, 5) << std::endl;
        // std::cout << "end : " <<_msg.substr(_msg.size() - 5) << std::endl;
        
    } else { /* TODO: check post response structure */
        _code = 204;
        _msg = "";
    }

    /* TODO: check why it need */
    if (_code == 500) _msg = load_html(_error_page[500]);

    _msg = make_header() + "\r\n" + _msg;
}

void Response::PUT() {
    std::string data;

    _msg = "";
    _code = write_data(_req._body);
    if (_code != 201 && _code != 204) _msg = load_html(_error_page[_code]);

    _msg = make_header() + "\r\n" + _msg;
}

void Response::DELETE() {
    if (ft_checkfile(_path.c_str()) == 1) {
        if (remove(_path.c_str()) == 0) _code = 204;
        else _code = 403;
    }
    else _code = 404;

    if (_code == 403 || _code == 404) _msg = load_html(_error_page[_code]);

    _msg = make_header() + "\r\n" + _msg;
}

/*
    TODO: is this same with HEAD?
*/
void Response::OPTIONS() {
    _code = read_data();
    
    _msg = make_header() + "\r\n";
}

void Response::TRACE() {
    _msg = _req._raw;
}

int Response::write_data(std::string data) {
    std::ofstream file;

    if (ft_checkfile(_path.c_str()) == 1) {
        file.open(_path.c_str());
        if (file.is_open() == false)
			return (403);
        file << data;
        file.close();
        return 204;
    } else {
        file.open(_path.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (file.is_open() == false)
			return (403);
		file << data;
		file.close();
		return (201);
    }
}
