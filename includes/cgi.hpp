#ifndef CGI_HPP
# define CGI_HPP
# include "request.hpp"
# include "config.hpp"
# include <string.h>
# include <sys/wait.h>
# include <cstdio>

class Cgi {
    public:
        std::map<std::string, std::string> _options;
        std::string _body;
        
        Cgi(Request &req, std::string path, long host, int port);
        char **_getEnvAsCstrArray() const;
        std::string executeCgi(const std::string& scriptName);
};

#endif
