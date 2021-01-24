#pragma once

#include <initializer_list>
#include <memory>
#include <stddef.h>
#include <string>
#include <vector>

#include <assimp/scene.h>
#include <spdlog/spdlog.h>

#include "nimble/IMesh.h"
#include "nimble/opengl-wrapper/VertexBufferFormat.h"

namespace Nimble {
template <typename T>
class Mesh : public IMesh {
	friend class MeshFactory;

public:
	typedef std::vector<T> VertexStorage;
	typedef std::vector<unsigned int> IndexStorage;

private:
	VertexStorage _vertices;
	IndexStorage _indices;
	size_t _numFaces;

public:
	// support numFaces, so we could draw lines or points if we want
	Mesh(VertexStorage vertices, IndexStorage indices, size_t numFaces)
	: _vertices(vertices), _indices(indices), _numFaces(numFaces) {
	}

	Mesh(std::initializer_list<float> vertices, std::initializer_list<unsigned int> indices)
	: _vertices(vertices), _indices(indices) {
	}

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

	const size_t GetNumBytes() const override {
		return _vertices.size() * T::SizeInBytes();
	}

	const void *GetData() const override {
		return (void *)(&_vertices[0]);
	}

	const size_t GetNumFaces() const override {
		return _numFaces;
	}

	const size_t GetFacesNumBytes() const override {
		return _indices.size() * sizeof(unsigned int);
	}
	const void *GetFaceData() const override {
		return (void *)(&_indices[0]);
	}

private:
	static std::shared_ptr<Mesh<Position>> BuildMesh_Position(const aiMesh *mesh) {
		spdlog::info("[Position]: Building mesh with {} verts and {} faces", mesh->mNumVertices,
					 mesh->mNumFaces);
		// Return a mesh where we only read off Positions & Index data
		std::vector<Position> verts(mesh->mNumVertices);

		for(size_t i = 0; i < mesh->mNumVertices; ++i) {
			Position p;
			p.x = mesh->mVertices[i].x;
			p.y = mesh->mVertices[i].y;
			p.z = mesh->mVertices[i].z;
			verts[i] = p;
		}

		std::vector<unsigned int> indices;
		for(size_t i = 0; i < mesh->mNumFaces; ++i) {
			for(size_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		return std::make_shared<Mesh<Position>>(verts, indices, mesh->mNumFaces);
	}

	static std::shared_ptr<Mesh<PositionNormal>> BuildMesh_PositionNormal(const aiMesh *mesh) {
		spdlog::info("[PositionNormal]: Building mesh with {} verts and {} faces",
					 mesh->mNumVertices, mesh->mNumFaces);
		// Return a mesh where we only read off Positions & Index data
		std::vector<PositionNormal> verts(mesh->mNumVertices);

		for(size_t i = 0; i < mesh->mNumVertices; ++i) {
			PositionNormal p;
			p.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			p.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			verts[i] = p;
		}

		std::vector<unsigned int> indices;
		for(size_t i = 0; i < mesh->mNumFaces; ++i) {
			for(size_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		return std::make_shared<Mesh<PositionNormal>>(verts, indices, mesh->mNumFaces);
	}

	static std::shared_ptr<Mesh<PositionNormalUv>> BuildMesh_PositionNormalUv(const aiMesh *mesh) {
		spdlog::info("[PositionNormalUv]: Building mesh with {} verts and {} faces",
					 mesh->mNumVertices, mesh->mNumFaces);

		std::vector<PositionNormalUv> verts;
		verts.resize(mesh->mNumVertices);

		for(size_t i = 0; i < mesh->mNumVertices; ++i) {
			verts[i] = PositionNormalUv(
				glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
				glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
				glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
		}

		std::vector<unsigned int> indices;
		for(size_t i = 0; i < mesh->mNumFaces; ++i) {
			for(size_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		return std::make_shared<Mesh<PositionNormalUv>>(verts, indices, mesh->mNumFaces);
	}
};

class MeshFactory {
public:
	// Factory
	static std::shared_ptr<IMesh> FromFile(const aiMesh *mesh) {
		// We assume a Mesh should always have position data, along with
		// index data. From there, we go from the more specific combinations
		// and work down to the Position/Index only

		// TODO: Add more attributes as we need them
		if(mesh->HasNormals() && mesh->HasTextureCoords(0)) {
			return Mesh<PositionNormalUv>::BuildMesh_PositionNormalUv(mesh);
		} else if (mesh->HasNormals()) {
			return Mesh<PositionNormal>::BuildMesh_PositionNormal(mesh);
		} else {
			return Mesh<Position>::BuildMesh_Position(mesh);
		}
	}
};

} // namespace Nimble