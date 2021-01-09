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

inline std::string FileReadAllText(const std::string &path) {
	std::ifstream f(path.c_str());
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

inline std::vector<uint8_t> FileReadBytes(const std::string& path) {
	std::ifstream infile(path, std::ios_base::binary);

	return std::vector<uint8_t>(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
}