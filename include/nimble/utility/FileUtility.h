#pragma once

#include <string>
#include <fstream>
#include <sstream>

inline std::string FileReadAllText(const char* path) {
	std::ifstream f(path);
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}