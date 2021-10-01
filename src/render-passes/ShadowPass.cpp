#include "nimble/render-passes/ShadowPass.h"
#include <GL/glew.h>
#include <nimble/scene-graph/DirectionalLightNode.h>
#include <nimble/scene-graph/DrawableNode.h>

using namespace Nimble;

ShadowPass::ShadowPass(int shadowWidth, int shadowHeight) : _shadow_width(shadowWidth), _shadow_height(shadowHeight) {
	_shadow_map.depth_texture = std::make_shared<Texture2D>();
	_shadow_map.depth_texture->Create((int)shadowWidth, (int)shadowHeight, GL_DEPTH_COMPONENT);
	_shader = ResourceManager::Get().GetShader("shadow");
	ASSERT(_shader, "Could not find shadow shader through resource manager");

	_fbo.Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadow_map.depth_texture->GetTextureHandle(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	_fbo.Unbind();
}

void ShadowPass::Draw(SceneState &state, const SceneGraph &sceneGraph) {
	/*
	 * This pass requires:
	 * 	- A Frame buffer for the depth target
	 * 	- A texture to hold the results
	 * 	- Every drawable in the scene which casts shadows
	 *
	 */
	glCullFace(GL_FRONT);
	auto directionalLights = sceneGraph.GetNodesByDerivedType<DirectionalLightNode>(SceneNodeType::DIRECTIONAL_LIGHT);
	ASSERT(directionalLights.size(), "ShadowPass requires at least one directional light");

	glViewport(0, 0, _shadow_width, _shadow_height);
	_fbo.Bind();
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw...
	std::list<DrawableNode *> drawables = sceneGraph.GetNodesByDerivedType<DrawableNode>(SceneNodeType::DRAWABLE);

	glm::mat4 lightSpaceMatrix = directionalLights.front()->GetDirectionalLight().GetLightSpaceMatrix();

	_shadow_map.light_space_matrix = lightSpaceMatrix;

	for(const auto &drawable : drawables) {
		if(!drawable->GetMaterial()->GetCastsShadows().value()) {
			continue;
		}
		drawable->GetVAO()->Bind();
		drawable->GetVB().Bind();
		drawable->GetIB().Bind();

		auto transform = drawable->GetTransformation();

		_shader->Use();
		_shader->SetUniform("model", transform.GetWorldMatrix());
		_shader->SetUniform("lightSpaceMatrix", lightSpaceMatrix);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(drawable->GetIB().GetNumFaces() * 3), GL_UNSIGNED_INT, nullptr);
		drawable->GetVAO()->Unbind();
	}

	_fbo.Unbind();

	// Done, share the texture out
}
