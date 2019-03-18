#include "nimble/Mesh.h"

Mesh::Mesh(VertexStorage vertices, IndexStorage indices) : _vertices(vertices), _indices(indices) {
}

Mesh::Mesh(std::initializer_list<float> vertices, std::initializer_list<unsigned int> indices)
: _vertices(vertices), _indices(indices) {
}

VertexStorage::value_type *Mesh::VertexData() {
	return _vertices.data();
}

size_t Mesh::VertexByteCount() {
	return _vertices.size() * sizeof(VertexStorage::value_type);
}

size_t Mesh::NumVertices() {
	return _vertices.size();
}

IndexStorage::value_type *Mesh::IndexData() {
	return _indices.data();
}

size_t Mesh::IndexByteCount() {
	return _indices.size() * sizeof(IndexStorage::value_type);
}

size_t Mesh::NumIndices() {
	return _indices.size();
}