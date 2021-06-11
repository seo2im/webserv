#include "server.hpp"

void Server::setup(Config config) {
    int ret;

    _server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_socket == 0) exit(ft_error("ERROR: make socket failed", 1));

    _max_fd = _server_socket;

    int opt = 1;
    ret = setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    if (ret == -1) exit(ft_error("ERROR: make socket failed", 1));

    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(config._host.c_str());
    _addr.sin_port = htons(config._port);
    
    ret = bind(_server_socket, (struct sockaddr *)&_addr, sizeof(_addr));
    if (ret == -1)  exit(ft_error("ERROR: bind socket failed", 1));

    ret = listen(_server_socket, CLIENT_SIZE);
    if (ret == -1)  exit(ft_error("ERROR: listen socket failed", 1));

    for (int i = 0; i < CLIENT_SIZE; i++) _client_socket[i] = 0;
}

void Server::reset(fd_set read, fd_set write) {
    FD_SET(_server_socket, &read);
    FD_SET(_server_socket, &write);

    for (int i = 0; i < _max_fd; i++) {
        if (_client_socket[i] > 0) {
            FD_SET(_client_socket[i], &read);
            if (_response.find(_client_socket[i]) != _response.end())
                FD_SET(_client_socket[i], &write);
        }
    }
}

void Server::accept_client() {
    int ret;

    int addrlen = sizeof(_addr);
    int new_client = accept(_server_socket, (struct sockaddr *)&_addr, (socklen_t *)&addrlen);
    if (ret == -1) exit(1); /* Throw Error */

    if (new_client > _max_fd) _max_fd = new_client;
    fcntl(new_client, F_SETFL, O_NONBLOCK);
    
    for (int i = 0; i < _max_fd; i++) {
        if (_client_socket[i] == 0) {
            _client_socket[i] = new_client;
            break;
        }
    }
}

void Server::reading(fd_set read, fd_set write) {
    int len = -1;
    char buffer[1001];

    for(int i = 0; i < _max_fd; i++) {
        if (FD_ISSET(_client_socket[i], &read)) {
            len = recv(_client_socket[i], buffer, 1000, 0);
            if (len == -1) exit(1); /* exit error */
            /* else if (len == 0) exit(1); disconnect client */
            else {
                buffer[len] = '\0';
                if (_request[_client_socket[i]].size() == 0)
                    _request[_client_socket[i]].reserve(100100);
                _request[_client_socket[i]] += buffer;
            }
        }
    }
    
    if (_request.size() != 0) {
        for (std::map<int, std::string>::iterator it = _request.begin(); it != _request
        .end(); it++) {
            /*
                안되면 받을때 까지 빙빙 돌게 만들어야 함
            */
            _response.insert(std::make_pair((*it).first, (*it).second));
        }
    }
}

void Server::writing(fd_set read, fd_set write) {
    int len;

    for (int i = 0; i < _max_fd; i++) {
        if (FD_ISSET(_client_socket[i], &write)) {
            std::string msg = _response[_client_socket[i]];
            len = send(_client_socket[i], msg.c_str(), msg.size(), MSG_NOSIGNAL);
            if (len == -1) exit(1); /* restart client */
            else if ((size_t)len < msg.size()) _response[_client_socket[i]] = msg.substr(len, msg.size());
            else _response.erase(_client_socket[i]);
        }
    }
}