#include "lib.hpp"

int ft_checkfile(const char *path) {
    struct stat s;
    if (stat(path, &s) == 0) {
        if (s.st_mode & S_IFDIR) return 0;
        else if (s.st_mode & S_IFREG) return 1;
    }
    return  0;
}
