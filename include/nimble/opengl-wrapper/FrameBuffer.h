#pragma once

namespace Nimble {

class FrameBuffer {
private:
	unsigned int _fbo;

public:
	FrameBuffer();

	void Bind();
	void Unbind();
};

} // namespace Nimble