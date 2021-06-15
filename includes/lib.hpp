#ifndef LIB_HPP
# define LIB_HPP
# include <string>
# include <iostream>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sstream>
# include <sys/time.h>
# include <vector>

# define DEV 1

int ft_error(std::string, int num);
bool ft_is_end_string(std::string str, std::string end);
std::vector<std::string> ft_split(std::string str, char c);
int ft_checkfile(const char *path);
std::string ft_num2string(size_t n);
std::string ft_gettime();
std::string ft_nextline(const std::string str, size_t i);

#endif
