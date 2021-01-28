#pragma once

#include <string>
#include <filesystem>

namespace Nimble {

struct FileModifiedEvent {
	std::filesystem::path filePath;

};

}