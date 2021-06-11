#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <vector>
# include <string>
# include <map>
# include <sys/socket.h>
# include "lib.hpp"

class Config {
    public:
        Config(std::string blocks);

        std::string _host;
        int         _port;

};

#endif
