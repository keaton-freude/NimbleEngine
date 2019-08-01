#include "nimble/scene-graph/SceneGraph.h"

using namespace Nimble;

SceneGraph::SceneGraph() {
}

SceneGraph::SceneGraph(RootSceneNode *node) : _rootNode(node) {
}

void SceneGraph::Render() {
	_rootNode->Render();
}

size_t SceneGraph::AddChildToRoot(SceneNode *node) {
	return _rootNode->AddChild(node);
}

size_t SceneGraph::AddChild(SceneNode *node, size_t id) {
	// Find the node with the correct id..
	auto parent = Find(id);
	if(!parent.has_value()) {
		throw std::runtime_error(fmt::format("Failed to find parent node for ID {}", id));
	}

	return parent.value()->AddChild(node);
}

size_t SceneGraph::AddChild(std::unique_ptr<SceneNode> &&node, size_t id) {
	auto parent = Find(id);
	if(!parent.has_value()) {
		throw std::runtime_error(fmt::format("Failed to find parent node for ID {}", id));
	}

	return parent.value()->AddChild(std::move(node));
}

const std::optional<SceneNode *const> SceneGraph::Find(size_t id) {
	if(_rootNode->GetID() == id)
		return _rootNode.get();

	return _rootNode->Find(id);
}