#include "config.hpp"

Config::Config() {
    init();
    _ports.push_back(80);
}

Config::Config(std::string block) {
    init();

    size_t start = 0;
    std::string line;
    std::vector<std::string> option;
    while ((line = ft_nextline(block, start)) != "") {
        option = ft_split(line, ' ');
        if (option.front() == "location") {
            start += (set_location(option.back(), block.substr(start + line.size() + 1, block.substr(start + line.size() + 1).find("\n\n"))) + line.size() + 1);
            continue;
        } else set_option(option.front(), option.back());
        
        start += line.size() + 1;
    }
}

int Config::set_location(std::string loc_path, std::string location_block) {
    Location location(location_block, _host, _ports, _root, _index, _error_page, _buffer_size, _methods);
    _locations[loc_path] = location;
    
    return location_block.size() + 2;
}

void Config::set_option(std::string key, std::string value) {
    if (key == "host") {
        _host = value;
    } else if (key == "root") {
        _root = value;
    } else if (key == "port") {
        std::vector<std::string> temp = ft_split(value, ',');
        for(std::vector<std::string>::iterator it = temp.begin(); it != temp.end(); it++) {
            _ports.push_back(atoi((*it).c_str()));
        }
    } else if (key == "index") {
        _index = value;
    } else if (key == "error_page") {
        _error_page[404] = value;
    } else if (key == "buffer_size") {
        _buffer_size = atoi(value.c_str());
    } else if (key == "name") {
        _name = value;
    } else if (key == "methods") {
        _methods = ft_split(value, ',');
    }
}

Config &Config::operator=(Config const &src) {
    _host = src._host;
    _ports = src._ports;
    _index = src._index;
    _error_page = src._error_page;
    _buffer_size = src._buffer_size;
    _locations = src._locations;
    _name = src._name;
    return (*this);
}

void Config::init() {
    _host = "127.0.0.1";
    _index = "index.html";
    _name = "";
    _buffer_size = std::string::npos;
    _error_page[400] = "./error/400.html";
    _error_page[403] = "./error/403.html";
    _error_page[404] = "./error/404.html";
    _error_page[405] = "./error/405.html";
    _error_page[410] = "./error/410.html";
    _error_page[413] = "./error/413.html";
    _error_page[500] = "./error/500.html";
}

std::ostream &operator<<(std::ostream &out, const Config &config) {
    out << "HOST :" << config._host << std::endl;
    out << "PORTS :";
    for (std::vector<int>::const_iterator it = config._ports.begin(); it != config._ports.end(); it++)
        out << (*it) << ",";
    out << std::endl;
    out << "METHODS :";
    for (std::vector<std::string>::const_iterator it = config._methods.begin(); it != config._methods.end(); it++)
        out << (*it) << ",";
    out << std::endl;
    out << "LOCATIONS :";
    for (std::map<std::string, Location>::const_iterator it = config._locations.begin(); it != config._locations.end(); it++)
        out << (*it).second << "\n";
    out << std::endl;
    return out;
}