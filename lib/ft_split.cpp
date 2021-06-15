#include "lib.hpp"

std::vector<std::string> ft_split(std::string str, char c) {
	std::vector<std::string> strs;
	
	size_t i = 0;
	size_t j = str.find_first_of(c);
	while (j != std::string::npos) {
		strs.push_back(str.substr(i, j + i));
		i += (j + 1);
		j = str.substr(i).find_first_of(c);
	}
	strs.push_back(str.substr(i));
	return strs;
}
