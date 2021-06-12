#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <string>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "config.hpp"
#include "lib.hpp"
# include <fcntl.h>

#define CLIENT_SIZE 100

class Server {
    public:
        long _host;
        long _port;

        int _server_socket;
        int _client_socket[CLIENT_SIZE];
        std::map<int, std::string> _request;
        std::map<int, std::string> _response;

        int _max_fd;

        struct sockaddr_in _addr;

    
    /* public: */
        void setup(Config config);
        void reset(fd_set &read, fd_set &write);
        void disconnect_all();
        void disconnect(int i);
        void accept_client();
        void reading(fd_set read, fd_set write);
        void writing(fd_set read, fd_set write);
};

#endif
