#include "nimble/subsystem/FileWatcherSubsystem.h"
#include <spdlog/spdlog.h>

#include <nimble/core/Assert.h>
#include <sstream>

#include <windows.h>

using namespace Nimble;

FileWatcherSubsystem::FileWatcherSubsystem(std::filesystem::path directory, ChangeType changeType)
	: _monitoredDirectory(directory), _changeType(changeType) {
}

void FileWatcherSubsystem::OnCreate() {
	spdlog::info("Watching for {0} events in {1}", ChangeTypeToString(_changeType),
				 _monitoredDirectory.string().c_str());

	_fileWatcherData.directoryHandle = UniqueHandle(CreateFileA((const char *)_monitoredDirectory.string().c_str(), GENERIC_READ,
											 FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
											 OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL));

	if (_fileWatcherData.directoryHandle.get() == INVALID_HANDLE_VALUE) {
		spdlog::error("Failed to create watcher subsystem. Not able to open directory. Error: {}", GetLastError());
	}

	_fileWatcherData.overlap.hEvent = (void*)this;
}

void FileWatcherSubsystem::OnTick(float dt) {
	(void)dt;
	ASSERT_NOT_NULL(_fileWatcherData.directoryHandle);

	static auto completionHandler = [](DWORD errorCode, DWORD numBytes, LPOVERLAPPED overlapped) {
		FileWatcherSubsystem* fws = static_cast<FileWatcherSubsystem*>(overlapped->hEvent);
		fws->OnDirectoryChanges();
	};

	if (!_fileWatcherData.waitingForChanges) {

		if(ReadDirectoryChangesW(_fileWatcherData.directoryHandle.get(), _fileWatcherData.buffer, FileInfoBufferSize, true,
								 FILE_NOTIFY_CHANGE_SIZE, 0, &_fileWatcherData.overlap, completionHandler) != 0) {
			_fileWatcherData.waitingForChanges = true;
		} else {
			spdlog::error("ReadDirectoryChanges failed with error {:#04x} Aborting tick", GetLastError());
		}
	} else {
		// Check for completion & handle
	}
}

void FileWatcherSubsystem::OnDestroy() {
}

void FileWatcherSubsystem::OnDirectoryChanges() {
	_fileWatcherData.waitingForChanges = false;
	// Decompose the buffer to find the changes
	auto * fileNotifyInformation = (FILE_NOTIFY_INFORMATION*)_fileWatcherData.buffer;

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
		fileNotifyInformation = (FILE_NOTIFY_INFORMATION*)(_fileWatcherData.buffer + fileNotifyInformation->NextEntryOffset);
	} while (fileNotifyInformation->NextEntryOffset);
}