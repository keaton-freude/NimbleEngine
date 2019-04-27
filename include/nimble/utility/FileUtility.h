#pragma once

#include <fstream>
#include <sstream>
#include <string>

std::string FileReadAllText(const char *path) {
	std::ifstream f(path);
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::string FileReadAllText(const std::string& path) {
	std::ifstream f(path.c_str());
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}