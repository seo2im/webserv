#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <vector>
# include <string>
# include <map>
# include <set>
# include <sys/socket.h>
# include "lib.hpp"

class Location {
    public: 
        std::string _host;
        int _port;
};

class Config {
    public:
        std::string _host;
        int _port;
        std::vector<std::string> _names;
        

        // std::map<int, std::string> _error_page;
        int _buffer_size;
        // std::map<std::string, std::string> _cgi_oprions;
        // std::string _cgi_pass;
        bool _autoindex;
        std::string _root;
        std::string _index;
        std::set<std::string> _allow_methods;
        std::string _alias;

        Config();
        Config(std::string blocks);
        Config & operator=(const Config & src);
};

#endif
