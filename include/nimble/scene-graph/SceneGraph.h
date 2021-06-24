#pragma once

/*
	NOTE: All of this subject to change

	A scene graph is a way of organzing translations, rotations, animations, rendering,
	object attachment and much more in a 3D scene.

	We have a variety of different Node Types
	RenderNode - Renders some attached geometry
	MaterialNode - Contains material settings to be applied
		Materials contain bound shaders, require Uniforms to be set, textures to be bound
		Along with OpenGL render state like blend mode, culling, whatever

		NOTE: We may need to limit this such that only 1 MaterialNode can be applied
		at once, or it somehow overwrites the previous one, because mixing materials is probably
		going to be confusing and not work well (how to pick a "mixed" shader?, vertex offsets
		etc)
	TransformNode - This Node modifies the SceneGraphs Current transform value
		A transform is some state of Translation, Rotation, Scale
	RotateNode - Apply some rotation to the current world matrix
	ScaleNode - Apply some scaling operation to the current world matrix
	TranslateNode - Apply a translation to the current world matrix

*/

#include "nimble/camera/Camera.h"
#include "nimble/core/Assert.h"
#include "nimble/scene-graph/RootSceneNode.h"
#include "nimble/scene-graph/SceneNode.h"
#include <glm/glm.hpp>
#include <list>
#include <memory>

namespace Nimble {

class SceneGraph {
private:
	// Probably convert this to some specific RootSceneNode
	// Maybe something which supports a variety of what will end up being
	// global transformations for the entire scene
	std::unique_ptr<RootSceneNode> _rootNode;

public:
	// Use the default root node type (All-axis transform)
	SceneGraph() = delete;

	SceneGraph(std::shared_ptr<glm::mat4> projectionMatrix, std::shared_ptr<ICamera> camera);

	// Apply the graph. Walk every node, applying the node
	// This can result in transformations, meshes being drawn, etc
	void Render();
	void RenderGui();

	// Take ownership of a raw pointer, similar to the
	// SceneNode::AddChild
	SceneNode::NodeIdRet AddChildToRoot(SceneNode *node);

	size_t AddChild(SceneNode *node, size_t id);
	size_t AddChild(std::unique_ptr<SceneNode> &&node, size_t id);

	template <typename... Args>
	void AddChildrenToRoot(Args... args) {
		static_assert((std::is_base_of_v<SceneNode, std::remove_pointer_t<Args>> && ...),
					  "One of the arguments passed to this function is not derived from SceneNode");
		_rootNode->AddChildren(args...);
	}

	RootSceneNode *GetRootNode() const {
		return _rootNode.get();
	}

	// Look through all nodes in the graph for `id`
	const std::optional<SceneNode *const> Find(size_t id);

	std::list<SceneNode *> GetNodesByType(SceneNodeType type) const;

	// Templated helper to automatically cast found nodes to `T*`
	template <typename T>
	std::list<T *> GetNodesByDerivedType(SceneNodeType type) const {
		ASSERT(T::SCENE_NODE_TYPE() == type, "Requested cast type ({}) does not match template parameter type ({})", T::SCENE_NODE_TYPE(), type);

		// walk scene graph looking for all nodes of @type and return
		std::list<T *> nodes{};

		std::function<void(SceneNodeType, SceneNode *)> func = [&nodes, &func](SceneNodeType type, SceneNode *node) {
			if(node->GetSceneNodeType() == type) {
				nodes.push_back(dynamic_cast<T *>(node));
			}

			for(auto &child : node->GetChildren()) {
				func(type, child.get());
			}
		};

		func(type, _rootNode.get());

		return nodes;
	}

	template <typename T>
	T *GetOneNodeByDerivedType(SceneNodeType type) const {
		// Return the first node we find with the requested type

		std::function<T *(SceneNodeType, SceneNode *)> func = [&func](SceneNodeType type, SceneNode *node) -> T * {
			if(node->GetSceneNodeType() == type) {
				return dynamic_cast<T *>(node);
			}

			for(auto &child : node->GetChildren()) {
				return func(type, child.get());
			}

			return nullptr;
		};

		T *result = func(type, _rootNode.get());

		return result;
	}
};

}; // namespace Nimble