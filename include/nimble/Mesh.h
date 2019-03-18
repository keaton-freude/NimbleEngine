#pragma once
#include <vector>

typedef std::vector<float> VertexStorage;
typedef std::vector<unsigned int> IndexStorage;

class Mesh {
private:
	VertexStorage _vertices;
	IndexStorage _indices;

public:
	Mesh() = default;
	Mesh(VertexStorage vertices, IndexStorage indices);
	Mesh(std::initializer_list<VertexStorage::value_type> vertices,
		 std::initializer_list<IndexStorage::value_type> indices);

	VertexStorage::value_type *VertexData();
	size_t VertexByteCount();
	size_t NumVertices();

	IndexStorage::value_type *IndexData();
	size_t IndexByteCount();
	size_t NumIndices();
};