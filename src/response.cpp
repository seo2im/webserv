#include "response.hpp"

Response::Response(Request &req, Config &config) {
    
}

std::string Response::to_string() {
    return "test OK\n";   
}