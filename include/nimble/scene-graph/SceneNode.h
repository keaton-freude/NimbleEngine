#pragma once

#include <list>
#include <memory>

/*
	SceneNode is the abstract base class for all SceneNodes
	It presents an API from which SceneGraph can do its work, including
	a way to deterministically determine the type of the derived types,
	without using dynamic_cast
*/

namespace Nimble {
class SceneNode {
private:
	std::list<std::unique_ptr<SceneNode>> _children;


public:
	virtual ~SceneNode() = default;

	// Apply this SceneNodes specific transformation or action
	virtual void Apply() = 0;

	void AddChild(std::unique_ptr<SceneNode> node) {
		_children.push_back(std::forward<std::unique_ptr<SceneNode>>(node));
	}

	template <typename T, typename... Args>
	void AddChild(Args &&... args) {
		_children.push_back(std::make_unique<T>(std::forward<Args>(args)...));
	}
};
} // namespace Nimble