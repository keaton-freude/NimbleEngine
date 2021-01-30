#include "nimble/subsystem/FileWatcherSubsystem.h"
#include <spdlog/spdlog.h>

#include <nimble/core/Assert.h>
#include <sstream>

#ifdef _WIN_32
#include <windows.h>
#endif

typedef void* HANDLE;

using namespace Nimble;

FileWatcherSubsystem::FileWatcherSubsystem(std::fileswitchsystem::path directory, ChangeType changeType)
	: _monitoredDirectory(directory), _changeType(changeType) {
}

void FileWatcherSubsystem::OnCreate() {
#ifdef _WIN32
	spdlog::info("Watching for {0} events in {1}", ChangeTypeToString(_changeType),
				 _monitoredDirectory.string().c_str());
	_directoryHandle = new HANDLE;

	*(HANDLE*)_directoryHandle = CreateFileA((const char *)_monitoredDirectory.string().c_str(), GENERIC_READ,
											 FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
											 OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);

	if (*(HANDLE*)_directoryHandle == INVALID_HANDLE_VALUE) {
		spdlog::error("Failed to create watcher subsystem. Not able to open directory. Error: {}", GetLastError());
	}

	_overlap.hEvent = (void*)this;
#endif
}

void FileWatcherSubsystem::OnTick(float dt) {
#ifdef _WIN32
	ASSERT_NOT_NULL(_directoryHandle);
	HANDLE directoryHandle = *(HANDLE*)_directoryHandle;

	static auto completionHandler = [](DWORD errorCode, DWORD numBytes, LPOVERLAPPED overlapped) {
		FileWatcherSubsystem* fws = static_cast<FileWatcherSubsystem*>(overlapped->hEvent);
		fws->OnDirectoryChanges();
	};

	if (!_waitingForChanges) {

		if(ReadDirectoryChangesW(directoryHandle, _buffer, 1048576, true,
								 FILE_NOTIFY_CHANGE_SIZE, 0, &_overlap, completionHandler) != 0) {
			_waitingForChanges = true;
		} else {
			spdlog::error("ReadDirectoryChanges failed. Aborting tick");
		}
	} else {
		// Check for completion & handle
	}
#endif
}

void FileWatcherSubsystem::OnDestroy() {
}

void FileWatcherSubsystem::OnDirectoryChanges() {
#ifdef _WIN32
	_waitingForChanges = false;
	// Decompose the buffer to find the changes
	auto * fileNotifyInformation = (FILE_NOTIFY_INFORMATION*)_buffer;

	do {
		// Find each change and create events from them
		switch (fileNotifyInformation->Action) {
		case FILE_ACTION_ADDED:
			spdlog::info("File added");
			break;
		case FILE_ACTION_REMOVED:
			spdlog::info("File removed");
			break;
		case FILE_ACTION_MODIFIED: {
			std::filesystem::path path = _monitoredDirectory / std::filesystem::path(
				std::wstring(fileNotifyInformation->FileName, fileNotifyInformation->FileNameLength));

			spdlog::info("{} modified", path.string().c_str());
			FileModifiedEvent.Emit(path);
			break;
		}
		case FILE_ACTION_RENAMED_OLD_NAME:
			spdlog::info("File renamed - old");
			break;
		case FILE_ACTION_RENAMED_NEW_NAME:
			spdlog::info("File renamed - new");
			break;
		default:
			spdlog::error("Unhandled action type");
		}
		fileNotifyInformation = (FILE_NOTIFY_INFORMATION*)(_buffer + fileNotifyInformation->NextEntryOffset);
	} while (fileNotifyInformation->NextEntryOffset);
#endif
}