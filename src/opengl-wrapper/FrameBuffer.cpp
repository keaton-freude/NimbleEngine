#include "nimble/opengl-wrapper/FrameBuffer.h"
#include "GL/glew.h"

using namespace Nimble;

FrameBuffer::FrameBuffer() {
	glGenFramebuffers(1, &_fbo);
}

void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void FrameBuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}