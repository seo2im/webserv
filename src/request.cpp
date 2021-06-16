# include "request.hpp"

Request::Request() {

}
Request::Request(std::string &raw) {
    if (raw.find("Transfer-Encoding: chunked") != std::string::npos &&
        raw.find("Transfer-Encoding: chunked") < raw.find("\r\n\r\n")) {
    	_raw = chunked_process(raw);
    } else _raw = raw;
    
    if (_raw != "") {
        init();
		parse();
    }
}

Request & Request::operator=(const Request & src) {
	_raw = src._raw;
	_uri = src._uri;
	_method = src._method;
	_header = src._header;
	_body = src._body;
	_code = src._code;
	_port = src._port;
	_cgi = src._cgi;
	return (*this);
}

void Request::init() {
    _method = "";
    _protocol = "";
    _code = 200;
    _body = "";
    _port = 80;
    _uri = "";
	//_query = ""; //TODO: What it is

	_header.clear();
    // _header["Accept-Charsets"] = "";
	// _header["Accept-Language"] = "";
	// _header["Allow"] = "";
	// _header["Auth-Scheme"] = "";
	// _header["Authorization"] = "";
	// _header["Content-Language"] = "";
	// _header["Content-Length"] = "";
	// _header["Content-Location"] = "";
	// _header["Content-Type"] = "";
	// _header["Date"] = "";
	// _header["Host"] = "";
	// _header["Last-Modified"] = "";
	// _header["Location"] = "";
	// _header["Referer"] = "";
	// _header["Retry-After"] = "";
	// _header["Server"] = "";
	// _header["Transfer-Encoding"] = "";
	// _header["User-Agent"] = "";
	// _header["Www-Authenticate"] = "";
	_header["Connection"] = "Keep-Alive";
}
/*
	TODO: Check it is right work!!
	=> for funny this is same from hbaudet!
*/
std::string Request::chunked_process(std::string &raw) {
	std::string head = raw.substr(0, raw.find("\r\n\r\n"));
	std::string	chunks = raw.substr(raw.find("\r\n\r\n") + 4, raw.size() - 1);
	std::string	subchunk = chunks.substr(0, 100);
	std::string	body = "";
	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;

	while (chunksize)
	{
		i = chunks.find("\r\n", i) + 2;
		body += chunks.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = chunks.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}

	return (head + "\r\n\r\n" + body + "\r\n\r\n");
}
/*
	TODO:
	1. set Lang & Query (have to control here)
*/
void Request::parse() {
	std::string temp = _raw;
	
	temp = parse_request_line(temp);
	temp = parse_header(temp);
	_body.assign(temp);
	if (_code != 200) {
		/*
			TODO: Ynot 200 is error when parse of request message
		*/
	}
}
/*
	TODO:
	1. method check
	2. uri check
	3. versioncheck
	4. etc...
*/
std::string Request::parse_request_line(std::string raw) {
	size_t i = raw.find("\r\n");
	std::string request_line = raw.substr(0, i);

	std::cout << "req :" << request_line << std::endl;

	size_t j = request_line.find_first_of(' ');
	if (j == std::string::npos) {
		/*
			400 page: No method rf
		*/
		_code = 400;
	}
	_method.assign(request_line, 0, j);
	request_line = request_line.substr(j + 2);

	j = request_line.find_first_of(' ');
	if (j == std::string::npos) {
		/*
			400 page: No uri rf
		*/
		_code = 400;
	}
	_uri.assign(request_line, 0, j);
	_protocol.assign(request_line, j + 1, i);

	return raw.substr(i + 2);
}
/*
	TODO
	1. CGI Check
	2. each header content check
*/
std::string Request::parse_header(std::string raw) {
	size_t i = raw.find("\r\n\r\n");
	if (i == std::string::npos) return "";
	std::string temp = raw.substr(0, i + 2);

	std::string line;
	size_t j;
	j = temp.find("\r\n");
	size_t k;
	while (j != std::string::npos && _code != 400) {
		line = temp.substr(0, j);
		
		k = line.find_first_of(':');
		if (k == std::string::npos) {
			/*
				400 page
			*/
			_code = 400;
		}
		_header[line.substr(0, k)] = line.substr(k + 2);
		temp = temp.substr(j + 2);
		j = temp.find("\r\n");
	}
	return raw.substr(i + 4);
}
