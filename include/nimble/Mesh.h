#pragma once

#include <initializer_list>
#include <stddef.h>
#include <vector>

template <typename T>
class Mesh {
public:
	typedef std::vector<T> VertexStorage;
	typedef std::vector<unsigned int> IndexStorage;

private:
	VertexStorage _vertices;
	IndexStorage _indices;

public:
	Mesh(VertexStorage vertices, IndexStorage indices) : _vertices(vertices), _indices(indices) {
	}

	Mesh(std::initializer_list<float> vertices, std::initializer_list<unsigned int> indices)
	: _vertices(vertices), _indices(indices) {
	}

	// Factory


	const VertexStorage &VertexData() {
		return _vertices;
	}

	size_t VertexByteCount() {
		return _vertices.size() * sizeof(VertexStorage::value_type);
	}

	size_t NumVertices() {
		return _vertices.size();
	}

	const IndexStorage &IndexData() {
		return _indices;
	}

	size_t IndexByteCount() {
		return _indices.size() * sizeof(IndexStorage::value_type);
	}

	size_t NumIndices() {
		return _indices.size();
	}
};
