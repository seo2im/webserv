#include "config.hpp"

Config::Config() {
    _host = "127.0.0.1";
    _port = 80;
}

Config::Config(std::string blocks) {
    _host = "127.0.0.1";
    if (blocks == "1") _port = 4000;

    _autoindex = false;
    _buffer_size = 1000000000;
    _root = "./YoupiBanane";
    _index = "index.html";
    _alias = "";
    _allow_methods.insert("GET");
    _allow_methods.insert("HEAD");
    std::cout << *(_allow_methods.find("GET")) << std::endl;
    // _cgi_pass;
    // _cgi_params;
    // _error_page;
}

Config & Config::operator=(const Config & src) {
    _host = src._host;
    _port = src._port;
    _names = src._names;
    _buffer_size = src._buffer_size;
    _autoindex = src._autoindex;
    _root = src._root;
    _index = src._index;
    _allow_methods = src._allow_methods;
    std::cout << "size: " << _allow_methods.size() << std::endl;
    _alias = src._alias;

    return (*this);
}
