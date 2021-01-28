#pragma once

namespace Nimble {

// Represents a system which can handle per-frame events
// and encapsulates some piece of functionality. Allows us
// to run code asynchronously across multiple frames
// Sub systems are always executed on the main-thread and
// care must be taken to not block wherever possible

// If a subsystem cannot carry out its task without blocking
// it should create a thread to handle the work and synchronize
// any work with the main-thread (the Tick function)
class ISubsystem {
public:
	virtual ~ISubsystem() = default;
	// Called when the subsystem is created.
	// Called a single time when the subsystem is first created
	virtual void OnCreate() = 0;

	// Called every frame. Input has been updated by the time `OnTick` is called
	// So current Input state can be queried and handled
	virtual void OnTick(float dt) = 0;

	// Called when the subsystem is removed from the current set of installed
	// subsystems
	virtual void OnDestroy() = 0;
};
}