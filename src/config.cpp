#include "config.hpp"

Config::Config() {
    _host = "127.0.0.1";
    _port = 80;
}

Config::Config(std::string blocks) {
    _host = "127.0.0.1";
    if (blocks == "1") _port = 4000;
    else if(blocks == "2") _port = 4001;
    else if(blocks == "3") _port = 4002;
    else if(blocks == "4") _port = 4003;
}

Config & Config::operator=(const Config & src) {
    _host = src._host;
    _port = src._port;

    return (*this);
}
