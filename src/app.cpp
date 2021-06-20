#include "app.hpp"

void App::init(char *filename) {
    std::vector<std::string> blocks = convertFile2Block(filename);
    for(std::vector<std::string>::iterator it = blocks.begin(); it != blocks.end(); it++) {
        Config config(*it);
        _configs.push_back(config);
    }
}

void App::setup() {
    for(std::vector<Config>::iterator it = _configs.begin(); it != _configs.end(); it++) {
        for (std::vector<int>::iterator jt = (*it)._ports.begin(); jt != (*it)._ports.end(); jt++) {
            Server serv((*it)._host, (*jt), (*it));
            _servers.push_back(serv);
        }
    }
}

void App::run() {
    fd_set read;
    fd_set write;

    std::vector<Server>::iterator server = _servers.begin();
    
    try {
        while (1) {
            try {
                FD_ZERO(&read);
                FD_ZERO(&write);
                make_connection(read, write);
                while (server != _servers.end()) {
                    (*server).reading(read);
                    (*server).writing(write);
                    server++;
                }
                server = _servers.begin();
            } catch (std::exception & e) {
                std::cout << e.what() << std::endl; // TODO: Set
            }
        }
    } catch (std::exception & e) {
        std::cout << e.what() << std::endl; // TODO: Set 
    }
}

void App::make_connection(fd_set &read, fd_set &write) {
    int ret;
    struct  timeval timeout = {1, 0};
    
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
        (*it).reset(read, write);
    }
    ret = select(FD_SETSIZE, &read, &write, NULL, &timeout);
    if (ret == -1) {
        std::cout << "CONNECTION FAILED: reconnected\n";
        make_connection(read, write);
    } 
    else if (ret == 0) {
       FD_ZERO(&read);
       FD_ZERO(&write);

       for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
            (*it).disconnect_all();
        }
        make_connection(read, write);
    } 
    else {
        for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++) {
            if (FD_ISSET((*it)._server_socket, &read)) {
                (*it).accept_client();
            }
        }
    }
}

/*
    convert file each server blocks string
*/
std::vector<std::string> App::convertFile2Block (char *filename) {
    std::vector<std::string> blocks;
    std::ifstream file;
    std::stringstream ss;
    std::string str;

    file.open(filename);
    ss << file.rdbuf();
    str = ss.str();
    file.close();
    
    size_t i;
    while (str != "") {
        i = str.find("\n\n\n");
        blocks.push_back(str.substr(0, i + 2));
        str = str.substr(i + 3);
    }
    return blocks;
}
