# include "request.hpp"

Request::Request(std::string &raw, Config &config) {
    std::cout << "raw req: " << raw << std::endl;
    std::cout << "check: " << config._host << std::endl;

    _raw = raw;
}

void Request::parse() {
    
}
