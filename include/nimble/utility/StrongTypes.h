#pragma once
#include <cstdint>

template <typename T, typename Tag>
class StrongType {
private:
	T _value;
public:
	explicit StrongType(T const& value) : _value(value) {}
	explicit StrongType(T && value) : _value(value) {}
	T& get() { return _value; }
	T const& get() const { return _value; }
};

using Width = StrongType<uint32_t, struct WidthTag>;
using Height = StrongType<uint32_t, struct HeightTag>;