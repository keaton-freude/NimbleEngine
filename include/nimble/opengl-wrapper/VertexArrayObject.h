#pragma once


// Wraps up a VAO, generates a new one when created
// lets user start recording commands, stop recording and then
// on-demand bind the state
class VertexArrayObject {
private:
	uint32_t _vaoHandle;
public:
	VertexArrayObject() {
		glGenVertexArrays(1, &_vaoHandle);
	}

	void StartRecording();
	void StopRecording();
	void Clear();
	void Bind();
};