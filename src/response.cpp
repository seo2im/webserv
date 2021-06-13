#include "response.hpp"

Response::Response(Request &req, Config &config) {
    std::cout << "raw req: " << req._raw << std::endl;
    std::cout << "check: " << config._host << std::endl;
}

std::string Response::to_string() {
    return "test OK\n";   
}