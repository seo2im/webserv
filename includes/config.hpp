#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <vector>
# include <string>
# include <map>
# include <sys/socket.h>
# include "lib.hpp"

class Config {
    public:
        std::string _host;
        int         _port;

        Config();
        Config(std::string blocks);
        Config & operator=(const Config & src);

};

#endif
