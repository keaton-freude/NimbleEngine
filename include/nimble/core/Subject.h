#pragma once

#include <list>
#include <atomic>

namespace Nimble {

template <typename T>
class Observer;

class Subscription;

template <typename T>
class Subject;

class ISubject;

template <typename T>
class Observer {
public:
	std::function<void(T)> observer{};
	size_t id{};
};

class ISubject {
public:
	~ISubject() = default;
	virtual void Unsubscribe(Subscription s) = 0;
};

class Subscription {
public:
	Subscription() = delete;
	Subscription(size_t id, ISubject* subject)
		: _id(id), _subject(subject) {}

	void Unsubscribe() {
		_subject->Unsubscribe(*this);
	}

	size_t GetID() const {
		return _id;
	}
private:
	size_t _id{};
	ISubject* _subject{};

};

// Provides a multi-cast Observable for some data of type `T`
template <typename T>
class Subject : public ISubject {
private:
	std::list<Observer<T>> _observers{};
public:
	Subscription Subscribe(std::function<void(T)> observer);
	void Unsubscribe(Subscription s) override {
		_observers.remove_if([s](Observer<T> obs) {
			return obs.id == s.GetID();
		});
	}

	void Emit(T t) {
		for (const auto& observer : _observers) {
			observer.observer(t);
		}
	}

};

template <typename T>
Subscription Subject<T>::Subscribe(std::function<void(T)> observer) {
	static std::atomic<size_t> id = 0;

	_observers.push_back({
		observer, id++
	});

	return {
		id, this
	};
}

}