#include "nimble/scene-graph/RootSceneNode.h"

using namespace Nimble;

RootSceneNode::RootSceneNode(std::shared_ptr<glm::mat4> projectionMatrix, std::shared_ptr<Camera> camera)
: _sceneState(projectionMatrix, camera) {
}
