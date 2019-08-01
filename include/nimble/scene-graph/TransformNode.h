#pragma once

/*
	Applies a transformation (translation, rotation, & scaling),
	such that all child-nodes will be affected
*/

#include "nimble/scene-graph/SceneNode.h"
#include "nimble/scene-graph/Transformation.h"

namespace Nimble {
class TransformNode : public SceneNode {
private:
	Transformation _transform;

public:
	TransformNode(Transformation transform) : _transform(transform) {
	}

	// NOTE: Add some factory methods to make creation easier, like:
	// TransformNode::FromRotation(axis, degrees)
	// TransformNode::FromTranslationRotation(vec3 translate, axis, degrees)
	// Etc

	virtual void Apply(Transformation &transformation) override {
		_transform.
	}
};
} // namespace Nimble