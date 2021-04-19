#pragma once

#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

#include "nimble/scene-graph/SceneState.h"

/*
	SceneNode is the abstract base class for all SceneNodes
	It presents an API from which SceneGraph can do its work, including
	a way to deterministically determine the type of the derived types,
	without using dynamic_cast
*/

namespace Nimble {

// Unknown type is just here for testing
enum class SceneNodeType { DIRECTIONAL_LIGHT, DRAWABLE, ROOT, UNKNOWN, NULL_NODE };

class SceneNode {
private:
	using NodeTy = std::unique_ptr<SceneNode>;
	using ChildrenStorageTy = std::vector<NodeTy>;

	ChildrenStorageTy _children;

	// A unique, continually increasing ID
	size_t _id;

	// Every scene node has a transform
	Transformation _transform{};

	void PropagateTranslation(const glm::vec3 &translation);
	void PropagateRotation(const glm::vec3 &axis, float rotation);
	void PropagateScale(const glm::vec3 &scale);

protected:
public:
	SceneNode();
	virtual ~SceneNode() = default;

	// The storage type of the Node's ID
	using NodeIdTy = size_t;
	using NodeIdRet = std::pair<SceneNode *, NodeIdTy>;

	// Apply this SceneNodes specific transformation or action
	// This function may modify the transformation, such that all child
	// nodes will be affected, but parent nodes won't
	// Because `Visit` is going to create copies, so they won't propagate back up
	virtual void Apply(SceneState &sceneState) = 0;

	// Traverse. First ourself, then our children. Only supporting pre-order traversal for now
	void Visit(SceneState sceneState);

	static SceneNodeType SCENE_NODE_TYPE() {
		return SceneNodeType::UNKNOWN;
	}

	virtual SceneNodeType GetSceneNodeType() {
		return SCENE_NODE_TYPE();
	}

	// Fluent API, return a reference to the inserted child after doing the insert
	// Also include the ID, in the pair
	NodeIdRet AddChild(SceneNode *node);
	NodeIdRet AddChild(NodeTy &&node);

	template <typename T, typename... Args>
	inline NodeIdRet AddChild(Args &&... args) {
		std::unique_ptr<SceneNode> child = new std::unique_ptr<T>(args...);
		child->_transform *= _transform;
		_children.push_back(new T(args...));

		return std::make_pair(_children.back().get(), _children.back()->GetID());
	}

	template <typename... Args>
	void AddChildren(Args... args) {
		static_assert((std::is_base_of_v<SceneNode, std::remove_pointer_t<Args>> && ...),
					  "One of the arguments passed to this function is not derived from SceneNode");

		// add the children
		((AddChild(args)), ...);
	}

	const ChildrenStorageTy &GetChildren() {
		return _children;
	}

	// Users should call GetID() on important nodes before they are giving over to the scene graph
	// This allows your systems to keep track of where important nodes are like:
	// The transform which governs an entire character, or, the Render node we would like to tweak
	// based on user input, etc
	// Guaranteed to be unique!
	size_t GetID() const;

	// Search ourself, and our children for the specific ID
	// Returning a raw pointer, because we can't use references in std::optional
	// Just have to hope users don't hold onto pointers for very long, OR always call
	// Find
	std::optional<SceneNode *> Find(size_t id);

	void Translate(glm::vec3 translation);
	void Rotate(glm::vec3 axis, float radians);
	void Scale(glm::vec3 scale);

	const Transformation &GetTransformation() const;

private:
	static size_t GenerateID();
};
} // namespace Nimble