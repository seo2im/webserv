#include "lib.hpp"

std::string pop(std::string str) {
	if (str.size())
		str.resize(str.size() - 1);
	return str;
}

std::string ft_nextline(const std::string str, size_t i) {
	std::string ret;
	size_t j;

	if (i == std::string::npos)
		return "";
	j = str.find_first_of('\n', i);
	ret = str.substr(i, j - i);
	if (ret[ret.size() - 1] == '\r')
		pop(ret);
	i = (j == std::string::npos ? j : j + 1);
	return ret;
}
