#include "nimble/subsystem/FileWatcherSubsystem.h"

#include "nimble/core/Assert.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>

using namespace Nimble;

FileWatcherSubsystem::FileWatcherSubsystem(std::filesystem::path directory, ChangeType changeType)
	: _monitoredDirectory(directory), _changeType(changeType) {
}

void FileWatcherSubsystem::OnCreate() {
	_fileWatcherData.fd = inotify_init();
	ASSERT_NE(_fileWatcherData.fd, -1);

	_fileWatcherData.wd = inotify_add_watch(_fileWatcherData.fd, _monitoredDirectory.string().c_str(),
											IN_MODIFY | IN_OPEN | IN_ACCESS | IN_CLOSE_WRITE);
	ASSERT_NE(_fileWatcherData.wd, -1);

	_fileWatcherData.epfd = epoll_create(16);


	_fileWatcherData.events.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
	_fileWatcherData.events.data.fd = _fileWatcherData.fd;

	ASSERT_NE(epoll_ctl(_fileWatcherData.epfd, EPOLL_CTL_ADD, _fileWatcherData.fd, &_fileWatcherData.events), -1);

	spdlog::info("Monitoring for file changes in: {}", _monitoredDirectory.string().c_str());
}

void FileWatcherSubsystem::OnTick(float dt) {
	int res = epoll_wait(_fileWatcherData.epfd, &_fileWatcherData.events, 1024, 0);

	ASSERT_NE(res, -1);

	if (res > 0) {
		OnDirectoryChanges();
	}
}

void FileWatcherSubsystem::OnDestroy() {
	close(_fileWatcherData.epfd);
	close(_fileWatcherData.wd);
	close(_fileWatcherData.fd);
}

void FileWatcherSubsystem::OnDirectoryChanges() {
	int i = 0;
	size_t n = read(_fileWatcherData.fd, _fileWatcherData.buffer, InotifyEventBufferSize);

	if (n == -1) {
		spdlog::error("Failed to read inotify event.");
		return;
	}

	if (n > 0) {
		while (i < n) {
			inotify_event* event = (inotify_event*)&_fileWatcherData.buffer[i];
			if (event->len) {
				if (event->mask & IN_CLOSE_WRITE) {
					// File changed event
					spdlog::info("{} modified", event->name);
					FileModifiedEvent.Emit(event->name);
				} else {
				}
			}
			i += sizeof(struct inotify_event) + event->len;
		}

	}
}