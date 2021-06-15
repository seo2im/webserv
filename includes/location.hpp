#ifndef LOCATION_HPP
# define LOCATION_HPP
#include "lib.hpp"
# include <vector>
# include <map>
# include <string>

class Location {
    public:
        std::string _host;
        std::vector<int> _ports;
        std::string _root;
        std::string _index;
        std::map<int, std::string> _error_page;
        int _buffer_size;
        std::string _upload_path;
        std::vector<std::string> _methods;
        
        Location() {};
        Location(
            std::string location_block,
            std::string host,
            std::vector<int> ports,
            std::string root,
            std::string index,
            std::map<int, std::string> error_page,
            int buffer_size,
            std::vector<std::string> _methods
        );
        Location &operator=(Location const & location);

        void parse_block(std::string block);
        void set_option(std::string key, std::string value);
};

std::ostream &operator<<(std::ostream &out, const Location &loc);

#endif