#include "cgi.hpp"

Cgi::Cgi(Request &req, std::string path, long host, int port) {
    _body = req._body;

    _options["REDIRECT_STATUS"] = "200";
    _options["GATEWAY_INTERFACE"] = "CGI/1.1";
    _options["SCRIPT_NAME"] = path;
    _options["SCRIPT_FILENAME"] = path;
    _options["REQUEST_METHOD"] = req._method;
    _options["CONTENT_LENGTH"] = ft_num2string(_body.length());
    _options["CONTENT_TYPE"] = req._header["Content-Type"];
    _options["PATH_INFO"] = "/" + req._uri; //might need some change, using config path/contentLocation
    _options["PATH_TRANSLATED"] = "/" + req._uri; //might need some change, using config path/contentLocation
    //_options["QUERY_STRING"] = request.getQuery();
    _options["REMOTEaddr"] = ft_num2string(host);
    _options["REMOTE_IDENT"] = req._header["Authorization"];
    _options["REMOTE_USER"] = req._header["Authorization"];
    _options["REQUEST_URI"] = "/" + req._uri; // + request.getQuery();
    if (req._header.find("Hostname") != req._header.end())
        _options["SERVER_NAME"] = req._header["Hostname"];
    else
        _options["SERVER_NAME"] = _options["REMOTEaddr"];
    _options["SERVER_PORT"] = ft_num2string(port);
    _options["SERVER_PROTOCOL"] = "HTTP/1.1";
    _options["SERVER_SOFTWARE"] = "Weebserv/1.0";
    if (req._header.count("X-Secret-Header-For-Test") > 0) {
        _options["HTTP_X_SECRET_HEADER_FOR_TEST"] = req._header["X-Secret-Header-For-Test"];
    }
        
}

char **Cgi::_getEnvAsCstrArray() const {
    char    **env = new char*[_options.size() + 1];
    int j = 0;
    for (std::map<std::string, std::string>::const_iterator i = _options.begin(); i != _options.end(); i++) {
        std::string element = i->first + "=" + i->second;
        env[j] = new char[element.size() + 1];
        env[j] = strcpy(env[j], (const char*)element.c_str());
        j++;
    }
    env[j] = NULL;
    return env;
}

std::string     Cgi::executeCgi(const std::string& scriptName) {
    pid_t pid;
    int saveStdin;
    int saveStdout;
    char **env;
    std::string newBody;

    try {
        env = this->_getEnvAsCstrArray();
    }
    catch (std::bad_alloc &e) {
        std::cerr << e.what() << std::endl;
    }

    // SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
    saveStdin = dup(STDIN_FILENO);
    saveStdout = dup(STDOUT_FILENO);

    FILE    *fIn = tmpfile();
    FILE    *fOut = tmpfile();
    long    fdIn = fileno(fIn);
    long    fdOut = fileno(fOut);
    int     ret = 1;

    write(fdIn, _body.c_str(), _body.size());
    lseek(fdIn, 0, SEEK_SET);

    pid = fork();

    if (pid == -1) {
        std::cerr << "Fork crashed." << std::endl;
        return ("Status: 500\r\n\r\n");
    } else if (!pid) {
        char * const * nll = NULL;

        dup2(fdIn, STDIN_FILENO);
        dup2(fdOut, STDOUT_FILENO);
        execve(scriptName.c_str(), nll, env);
        std::cerr << "Execve crashed." << std::endl;
        write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
    } else {
        char buffer[65536] = {0};

        waitpid(-1, NULL, 0);
        lseek(fdOut, 0, SEEK_SET);

        ret = 1;
        while (ret > 0)
        {
            memset(buffer, 0, 65536);
            ret = read(fdOut, buffer, 65535);
            newBody += buffer;
        }
    }

    dup2(saveStdin, STDIN_FILENO);
    dup2(saveStdout, STDOUT_FILENO);
    fclose(fIn);
    fclose(fOut);
    close(fdIn);
    close(fdOut);
    close(saveStdin);
    close(saveStdout);

    for (size_t i = 0; env[i]; i++)
        delete[] env[i];
    delete[] env;

    if (!pid)
        exit(0);

    return (newBody);
}
