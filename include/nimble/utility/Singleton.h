/*
 * Generic Singleton type
 *
 * Provides a Singleton<T> where T is the wrapped type
 */

template <typename T>
class Singleton {
public:
	virtual ~Singleton() = default;
	static T& Get() {
		static T _instance;
		return _instance;
	}
protected:
	Singleton() = default;
private:
	Singleton(Singleton const &) = delete;
	Singleton& operator=(Singleton const &) = delete;
};
