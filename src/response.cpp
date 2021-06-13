#include "response.hpp"

Response::Response(Request &req, Config &config) {
    _req = req;
    _config = _config;
    _code = req._code;
}

std::string Response::to_string() {
    return "test OK\n";   
}

void Response::GET() {
    if (false/* cgi flag */) {

    }
    else if (_code == 200) _code = read_data();
    else _msg = "Error code page"; /* TODO: load each code page */

    /* TODO: check why it need */
    if (_code == 500) _msg = "500"; /* TODO: load 500 page */

    _msg = make_header() + "\r\n" + _msg;
}

void Response::HEAD() {
    _code = read_data();
    _msg = make_header() + "\r\n";
}
/*
    TODO: Check why it no parse of body...
*/
void Response::POST() {
    if (false/* cgi flag */) {

    } else { /* TODO: check post response structure */
        _code = 204;
        _msg = "";
    }

    /* TODO: check why it need */
    if (_code == 500) _msg = "500"; /* TODO: load 500 page */

    _msg = make_header() + "\r\n" + _msg;
}

void Response::PUT() {
    std::string data;

    _msg = "";
    _code = write_data(_req._body);
    if (_code != 201 && _code != 204) _msg = "Error page"; /* TODO: load error page */

    _msg = make_header() + "\r\n" + _msg;
}

void Response::DELETE() {
    if (ft_checkfile(_path.c_str())) {
        if (remove(_path.c_str()) == 0) _code = 204;
        else _code = 403;
    }
    else _code = 404;

    if (_code == 403 || _code == 404) _msg = "Error page"; /* TODO: load error page */

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

int Response::read_data() {
    std::ifstream file;
    std::stringstream buffer;
    
    if (ft_checkfile(_path.c_str())) {
        file.open(_path.c_str(), std::ifstream::in);
        /* file fail => 403 */
        if (!file.is_open()) {
            _msg = "403"; /* TODO: 403 page load */
            return 403;
        }

        buffer << file.rdbuf();
        _msg = buffer.str();

        file.close();
        return 200; /* TODO: check right type */
    }
    else if (_is_autoindex) {
        _msg = "Authindex"; /* TODO: Set AutoIndex */
        _type = "text/html";
        return 200;
    }
    else {
        _msg = "404"; /* TODO: 404 page load */
        return 404;
    }
}

int Response::write_data(std::string data) {
    std::ofstream file;

    if (ft_checkfile(_path.c_str())) {
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
            + /* if error insert error in herer*/ " \r\n";
    for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); it++) {
        if ((*it).second != "") {
            header_msg += ((*it).first + ": " + (*it).second + "\r\n");
        }
    }

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
}

std::string Response::set_retry() {
    if (_code == 503 || _code == 429 || _code == 301) return "3"; /* TODO: check 3s is standard */
}

std::string Response::set_auth() {
    if (_code == 401) return "Basic realm=\"Access requires authentification\" charset=\"UTF-8\"";
}