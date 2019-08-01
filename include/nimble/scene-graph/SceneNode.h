#pragma once

#include <memory>
#include <optional>
#include <spdlog/spdlog.h>
#include <vector>

#include "nimble/scene-graph/Transformation.h"

/*
	SceneNode is the abstract base class for all SceneNodes
	It presents an API from which SceneGraph can do its work, including
	a way to deterministically determine the type of the derived types,
	without using dynamic_cast
*/

namespace Nimble {
class SceneNode {
private:
	using NodeTy = std::unique_ptr<SceneNode>;
	using ChildrenStorageTy = std::vector<NodeTy>;

	ChildrenStorageTy _children;

	// A unique, continually increasing ID
	size_t _id;

public:
	SceneNode();
	virtual ~SceneNode() = default;

	// Apply this SceneNodes specific transformation or action
	// This function may modify the transformation, such that all child
	// nodes will be affected, but parent nodes won't
	// Because `Visit` is going to create copies, so they won't propogate back up
	virtual void Apply(Transformation &transformation) = 0;

	// Traverse. First ourself, then our children. Only supporting pre-order traversal for now
	void Visit(Transformation transformation);

	// Take ownership of some existing raw SceneNode pointer
	size_t AddChild(SceneNode *node);

	// Take ownership of some existing SceneNode unique_ptr
	size_t AddChild(std::unique_ptr<SceneNode> &&node);

	// Construct a new child, forwarding the type T's args directly in
	template <typename T, typename... Args>
	inline size_t AddChild(Args &&... args) {
		_children.push_back(new T(args...));
		return _children.back()->GetID();
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
	const std::optional<SceneNode *const> Find(size_t id);

private:
	static size_t GenerateID();
};
} // namespace Nimble