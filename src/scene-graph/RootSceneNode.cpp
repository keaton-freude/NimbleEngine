#include "nimble/scene-graph/RootSceneNode.h"

using namespace Nimble;

RootSceneNode::RootSceneNode(std::shared_ptr<glm::mat4> projectionMatrix, std::shared_ptr<ICamera> camera)
: SceneNode("Scene Tree"), _sceneState(projectionMatrix, camera) {
}
