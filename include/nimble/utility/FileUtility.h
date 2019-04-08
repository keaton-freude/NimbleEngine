#pragma once

#include <fstream>
#include <sstream>
#include <string>

inline std::string FileReadAllText(const char *path) {
	std::ifstream f(path);
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}