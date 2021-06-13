#ifndef LIB_HPP
# define LIB_HPP
# include <string>
# include <iostream>
# include <unistd.h>
# include <stdlib.h>

# define DEV 1

int ft_error(std::string, int num);
bool ft_is_end_string(std::string str, std::string end);
char **ft_split(char const *s, char c);

#endif
