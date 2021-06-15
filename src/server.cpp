#include "server.hpp"

Server::Server(std::string host, int port, Config &config) {
    std::cout << host << ":" << port << " is setup...";
    _names = config._names;
    _root = config._root;
    _index = config._index;
    _error_page = config._error_page;
    _buffer_size = config._buffer_size;
    _locations = config._locations;
    
    int ret;

    _server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_socket == 0) exit(ft_error("ERROR: make socket failed", 1));

    _max_fd = _server_socket;

    int opt = 1;
    ret = setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    if (ret == -1) exit(ft_error("ERROR: make socket failed", 1));

    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(host.c_str());
    _addr.sin_port = htons(port);
    
    ret = bind(_server_socket, (struct sockaddr *)&_addr, sizeof(_addr));
    if (ret == -1)  exit(ft_error("ERROR: bind socket failed", 1));

    ret = listen(_server_socket, CLIENT_SIZE);
    if (ret == -1)  exit(ft_error("ERROR: listen socket failed", 1));

    for (int i = 0; i < CLIENT_SIZE; i++) _client_socket[i] = 0;
    std::cout << "Server " << host << ":" << port << " is Open!!" << std::endl;
}

void Server::reset(fd_set &read, fd_set &write) {
    FD_SET(_server_socket, &read);
    FD_SET(_server_socket, &write);

    for (int i = 0; i < CLIENT_SIZE; i++) {
        if (_client_socket[i] > 0) {
            FD_SET(_client_socket[i], &read);
            if (_response.find(_client_socket[i]) != _response.end())
                FD_SET(_client_socket[i], &write);
        }
    }
}

void Server::disconnect_all() {
    for (int i; i < _max_fd; i++) {
        if (_client_socket[i] > 0) disconnect(i);
    }   
}

void Server::disconnect(int i) {
    _request.erase(_client_socket[i]);
    _response.erase(_client_socket[i]);
    close(_client_socket[i]);
    _client_socket[i] = 0;
}

void Server::accept_client() {
    int ret;

    int addrlen = sizeof(_addr);
    int new_client = accept(_server_socket, (struct sockaddr *)&_addr, (socklen_t *)&addrlen);
    if (ret == -1) exit(ft_error("ERROR: Accept client error", 1)); /* Throw Error */

    if (new_client > _max_fd) _max_fd = new_client;
    fcntl(new_client, F_SETFL, O_NONBLOCK);
    
    for (int i = 0; i < _max_fd; i++) {
        if (_client_socket[i] == 0) {
            _client_socket[i] = new_client;
            break;
        }
    }
}

void Server::recv_from_client(fd_set &read, fd_set &write) {
    int len = -1;
    char buffer[1001];

    for(int i = 0; i < _max_fd; i++) {
        if (FD_ISSET(_client_socket[i], &read)) {
            len = recv(_client_socket[i], buffer, 1000, 0);
            if (len == -1) exit(ft_error("ERROR: recv error", 1)); /* TODO: client restart */
            else if (len == 0) disconnect(i);
            else {
                buffer[len] = '\0';
                if (_request[_client_socket[i]].size() == 0)
                    _request[_client_socket[i]].reserve(1001);
                _request[_client_socket[i]]+= buffer;
            }
        }
    }
}

void Server::reading(fd_set & read, fd_set & write) {
    recv_from_client(read, write);

    if (_request.size() != 0) {
        for (std::map<int, std::string>::iterator it = _request.begin(); it != _request.end(); it++) {
            size_t	i = (*it).second.find("\r\n\r\n");
            if (i != std::string::npos) {
                if ((*it).second.find("Content-Length: ") == std::string::npos) {
                    if ((*it).second.find("Transfer-Encoding: chunked") != std::string::npos) {
                        if (ft_is_end_string((*it).second, "0\r\n\r\n")) {
                            make_response((*it).first, (*it).second);
                            return ;
                        } else {
                            return ;
                        }
                    }
                    else {
                        make_response((*it).first, (*it).second);
                        return ;
                    }
                }

                size_t len = std::atoi((*it).second.substr((*it).second.find("Content-Length ") + 16, 10).c_str());
                if ((*it).second.size() >= len + i + 4) {
                    make_response((*it).first, (*it).second);
                    return ;
                }
                else return ;
            }
        }
    }
}

void Server::make_response(int fd, std::string raw) {
    Request request(raw);
    Response res(request, _locations);
    _response.insert(std::make_pair(fd, res.to_string()));
}

void Server::writing(fd_set &read, fd_set &write) {
    int len;

    for (int i = 0; i < _max_fd; i++) {
        if (FD_ISSET(_client_socket[i], &write)) {
            std::string msg = _response[_client_socket[i]];
            len = send(_client_socket[i], msg.c_str(), msg.size(), MSG_NOSIGNAL);
            if (len == -1) exit(ft_error("ERROR: send error", 1)); /* restart client */
            else if ((size_t)len < msg.size()) _response[_client_socket[i]] = msg.substr(len, msg.size());
            else {
                _request.erase(_client_socket[i]);
                _response.erase(_client_socket[i]);
            }
        }
    }
}