#ifndef APP_HPP
# define APP_HPP
# include <vector>
# include <map>
# include "server.hpp"
# include "config.hpp"
# include <sys/time.h>

class App {
    private:
        std::vector<Config> _configs;
        std::vector<Server> _servers;
    
    public:
        void init(char *filename);
        void setup();
        void run();
        void make_connection(fd_set &read, fd_set &write);

        std::vector<std::string> convertFile2Block(char *filename);
        

};

#endif
