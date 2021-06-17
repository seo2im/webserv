#ifndef CONFIG_HPP
# define CONFIG_HPP
#include "lib.hpp"
# include <vector>
# include <map>
# include <string>
# include "location.hpp"

class Config {
    public:
        std::string _host;
        std::vector<int> _ports;
        std::vector<std::string> _names;
        std::string _root;
        std::string _index;
        std::map<int, std::string> _error_page;
        size_t _buffer_size;
        std::vector<std::string> _methods;
        std::map<std::string, Location> _locations;

        Config();
        Config(std::string block);
        Config &operator=(Config const &src);

        void init();
        int set_location(std::string loc_path, std::string location_block);
        void set_option(std::string key, std::string value);
};

std::ostream &operator<<(std::ostream &out, const Config &config);

#endif
