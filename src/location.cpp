#include "location.hpp"

Location::Location(
            std::string location_block,
            std::string host,
            std::vector<int> ports,
            std::string root,
            std::string index,
            std::map<int, std::string> error_page,
            int buffer_size,
            std::vector<std::string> methods
            )
{
    _host = host;
    _ports = ports;
    _root = root;
    _index = index;
    _error_page = error_page;
    _buffer_size = buffer_size;
    _methods = methods;

    parse_block(location_block + "\n");
}

void Location::parse_block(std::string block) {
    size_t start = 0;
    std::string line;
    std::vector<std::string> option;

    while ((line = ft_nextline(block, start)) != "") {
        option = ft_split(line, ' ');
        set_option(option.front(), option.back());
        start += line.size() + 1;
    }
}

void Location::set_option(std::string key, std::string value) {
    if (key == "index") {
        _index = value;
    } else if (key == "error_page") {
        _error_page[404] = value;
    } else if (key == "buffer_size") {
        _buffer_size = atoi(value.c_str());
    } else if (key == "error_page") {
        _error_page[404] = value;
    } else if ("upload") {
        _upload_path = value;
    } else if ("methods") {
        _methods = ft_split(value, ',');
    }
}

Location &Location::operator=(Location const & src) {
    _host = src._host;
    _ports = src._ports;
    _root = src._root;
    _index = src._index;
    _error_page = src._error_page;
    _buffer_size = src._buffer_size;
    _upload_path = src._upload_path;
    _methods = src._methods;
    return (*this);
}