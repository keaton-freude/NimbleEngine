#pragma once

#include <initializer_list>
#include <memory>
#include <stddef.h>
#include <string>
#include <utility>
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
	size_t _numFaces{};
	std::shared_ptr<VertexArrayObject> _vao;

public:
	// support numFaces, so we could draw lines or points if we want
	Mesh(VertexStorage vertices, IndexStorage indices, size_t numFaces, std::shared_ptr<VertexArrayObject> vao)
	: _vertices(vertices), _indices(std::move(indices)), _numFaces(numFaces), _vao(std::move(vao)) {
	}

	Mesh(std::initializer_list<float> vertices, std::initializer_list<unsigned int> indices, std::shared_ptr<VertexArrayObject> vao)
	: _vertices(vertices), _indices(indices), _vao(std::move(vao)) {
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

	[[nodiscard]] size_t GetNumBytes() const override {
		return _vertices.size() * T::SizeInBytes();
	}

	[[nodiscard]] const void *GetData() const override {
		return (void *)(&_vertices[0]);
	}

	[[nodiscard]] size_t GetNumFaces() const override {
		return _numFaces;
	}

	[[nodiscard]] size_t GetFacesNumBytes() const override {
		return _indices.size() * sizeof(unsigned int);
	}
	[[nodiscard]] const void *GetFaceData() const override {
		return (void *)(&_indices[0]);
	}

	[[nodiscard]] std::shared_ptr<VertexArrayObject> GetVao() const override {
		return _vao;
	}

	static std::shared_ptr<VertexArrayObject> CreateVao() {
		auto vao = std::make_shared<VertexArrayObject>(&T::SetVertexAttribPointers);
		vao->Bind();
		vao->Unbind();
		return vao;
	}

private:
	static std::shared_ptr<Mesh<Position>> BuildMesh_Position(const aiMesh *mesh) {
		spdlog::info("[Position]: Building mesh with {} verts and {} faces", mesh->mNumVertices, mesh->mNumFaces);
		// Return a mesh where we only read off Positions & Index data
		std::vector<Position> verts(mesh->mNumVertices);

		for(size_t i = 0; i < mesh->mNumVertices; ++i) {
			Position p{};
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

		return std::make_shared<Mesh<Position>>(verts, indices, mesh->mNumFaces, Mesh<Position>::CreateVao());
	}

	static std::shared_ptr<Mesh<PositionNormal>> BuildMesh_PositionNormal(const aiMesh *mesh) {
		spdlog::info("[PositionNormal]: Building mesh with {} verts and {} faces", mesh->mNumVertices, mesh->mNumFaces);
		// Return a mesh where we only read off Positions & Index data
		std::vector<PositionNormal> verts(mesh->mNumVertices);

		for(size_t i = 0; i < mesh->mNumVertices; ++i) {
			PositionNormal p{};
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

		return std::make_shared<Mesh<PositionNormal>>(verts, indices, mesh->mNumFaces, Mesh<PositionNormal>::CreateVao());
	}

	static std::shared_ptr<Mesh<PositionColor>> BuildMesh_PositionColor(const aiMesh *mesh) {
		spdlog::info("[PositionColor]: Building mesh with {} verts and {} faces", mesh->mNumVertices, mesh->mNumFaces);
		std::vector<PositionColor> verts(mesh->mNumVertices);

		for(size_t i = 0; i < mesh->mNumVertices; ++i) {
			PositionColor p{};
			p.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			p.color = glm::vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a);
			verts[i] = p;
		}

		std::vector<unsigned int> indices;
		for(size_t i = 0; i < mesh->mNumFaces; ++i) {
			for(size_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		return std::make_shared<Mesh<PositionColor>>(verts, indices, mesh->mNumFaces, Mesh<PositionColor>::CreateVao());
	}

	static std::shared_ptr<Mesh<PositionNormalUv>> BuildMesh_PositionNormalUv(const aiMesh *mesh) {
		spdlog::info("[PositionNormalUv]: Building mesh with {} verts and {} faces", mesh->mNumVertices, mesh->mNumFaces);

		std::vector<PositionNormalUv> verts;
		verts.resize(mesh->mNumVertices);

		for(size_t i = 0; i < mesh->mNumVertices; ++i) {
			verts[i] = PositionNormalUv(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
										glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
										glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
		}

		std::vector<unsigned int> indices;
		for(size_t i = 0; i < mesh->mNumFaces; ++i) {
			for(size_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		return std::make_shared<Mesh<PositionNormalUv>>(verts, indices, mesh->mNumFaces, Mesh<PositionNormalUv>::CreateVao());
	}
};

// Tag for mesh loading to fall back to querying mesh details to try and figure
// out the type of the underlying data which is more expensive and potentially impossible
// to do correctly
struct AutoDetectVertexType;

class MeshFactory {
public:
	// Factory
	template <typename VertexType = AutoDetectVertexType>
	static std::shared_ptr<IMesh> FromFile(const aiMesh *mesh) {
		if constexpr(std::is_same_v<VertexType, PositionNormalUv>) {
			return Mesh<PositionNormalUv>::BuildMesh_PositionNormalUv(mesh);
		} else if constexpr(std::is_same_v<VertexType, PositionNormal>) {
			return Mesh<PositionNormal>::BuildMesh_PositionNormal(mesh);
		} else if constexpr(std::is_same_v<VertexType, PositionColor>) {
			return Mesh<PositionColor>::BuildMesh_PositionColor(mesh);
		} else if constexpr(std::is_same_v<VertexType, AutoDetectVertexType>) {
			if(mesh->HasNormals() && mesh->HasTextureCoords(0)) {
				return Mesh<PositionNormalUv>::BuildMesh_PositionNormalUv(mesh);
			} else if(mesh->HasNormals()) {
				return Mesh<PositionNormal>::BuildMesh_PositionNormal(mesh);
			} else {
				return Mesh<Position>::BuildMesh_Position(mesh);
			}
		} else {
			return Mesh<Position>::BuildMesh_Position(mesh);
		}
	}
};

} // namespace Nimble