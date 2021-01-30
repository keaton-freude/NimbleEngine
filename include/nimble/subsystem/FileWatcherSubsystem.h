#pragma once

#ifdef _WIN_32
#include <Windows.h>
#endif

#include "nimble/subsystem/Subsystem.h"
#include <filesystem>
#include <functional>
#include <string>
#include "nimble/core/Subject.h"

typedef void* OVERLAPPED;

namespace Nimble {

// Notify on what type of changes
enum class ChangeType : unsigned int {
	// Defined as a change to the last write time of the file. Does
	// not necessarily mean the contents of the file has changed
	FILE_CHANGED = 1,
	// File has been added to the subdirectory(s)
	FILE_ADDED = 2,
	// File has been removed from the subdirectory(s)
	FILE_REMOVED = 4,
	// File has been renamed
	FILE_RENAMED = 8
};

constexpr ChangeType operator|(ChangeType a, ChangeType b) {
	typedef std::underlying_type<ChangeType>::type enum_type;
	return static_cast<ChangeType>(static_cast<enum_type>(a) | static_cast<enum_type>(b));
}

// Shape of the event emitted when a file change modification is detected
using FileChangedEvent = std::function<void(ChangeType, std::string)>;

class FileWatcherSubsystem : public ISubsystem {
public:
	FileWatcherSubsystem() = delete;
	FileWatcherSubsystem(std::filesystem::path directory, ChangeType changeType);
	void OnCreate() override;
	void OnTick(float dt) override;
	void OnDestroy() override;

	Subject<std::filesystem::path> FileModifiedEvent{};

private:
	constexpr inline const char* ChangeTypeToString(ChangeType type) {
		switch (type) {
		case ChangeType::FILE_CHANGED:
			return "FILE_CHANGED";
		case ChangeType::FILE_ADDED:
			return "FILE_ADDED";
		case ChangeType::FILE_REMOVED:
			return "FILE_REMOVED";
		case ChangeType::FILE_RENAMED:
			return "FILE_RENAMED";
		default:
			return "UNKNOWN";
		}
	}

	void OnDirectoryChanges();

private:
	// The directory we are monitoring for changes
	std::filesystem::path _monitoredDirectory;
	ChangeType _changeType = ChangeType::FILE_CHANGED;
	void* _directoryHandle = nullptr;
	char _buffer[1048576]{};
	OVERLAPPED _overlap{};
	bool _waitingForChanges = false;
};

}