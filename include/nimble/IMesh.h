#pragma once

#include <cstdint>
#include <memory>

// IMesh is an interface over the Mesh class where
// the underlying templated type is hidden from the user

// For now.. user only cares about: how many bytes are needed
// to represent this mesh (including padding, if necessary) in the
// vertex buffer. Derived class needs to set this value
// And, a void* to the beginning of the data. Maybe this can be made
// typesafe.. but I really want to avoid exposing the underlying templated
// type to the user.. so that a user can do something like:

// IMesh *mesh = ResourceManager::Get().GetMesh("suzanne");
// And immediately feed this into a VertexBuffer object. Because the model
// is an external model, its not clear what the type T would be, without knowing
// about what kind of data is actually available in the Mesh

// Instead, we will want to extract all of the available data in the mesh

// If we want a stripped down version (maybe our shader doesn't care about tangents)
// The creation of the VertexBuffer can selectively decide what pieces to take
// But our Mesh class should have all of the data loaded off disk, into memory
// and not be tied to any assimp classes (in case we want to change)

#include "nimble/opengl-wrapper/VertexArrayObject.h"

namespace Nimble {
class IMesh {
public:
	[[nodiscard]] virtual size_t GetNumBytes() const = 0;
	[[nodiscard]] virtual const void *GetData() const = 0;

	[[nodiscard]] virtual size_t GetNumFaces() const = 0;
	[[nodiscard]] virtual size_t GetFacesNumBytes() const = 0;
	[[nodiscard]] virtual const void *GetFaceData() const = 0;

	[[nodiscard]] virtual std::shared_ptr<VertexArrayObject> GetVao() const = 0;

	virtual ~IMesh() = default;
};
} // namespace Nimble