#include "nimble/opengl-wrapper/Sampler.h"

using namespace Nimble;

const std::string SamplerConstants::ClampToEdgeStr = std::string("ClampToEdge");
const std::string SamplerConstants::RepeatStr = std::string("Repeat");
const std::string SamplerConstants::MirroredRepeat = std::string("MirroredRepeat");

const std::string SamplerConstants::NearestStr = std::string("Nearest");
const std::string SamplerConstants::LinearStr = std::string("Linear");

const std::string SamplerConstants::NearestMipmapNearestStr = std::string("NearestMipmapNearest");
const std::string SamplerConstants::LinearMipmapNearestStr = std::string("LinearMipmapNearest");
const std::string SamplerConstants::NearestMipmapLinearStr = std::string("NearestMipmapLinear");
const std::string SamplerConstants::LinearMipmapLinearStr = std::string("LinearMipmapLinear");

void Sampler::SetSettings(SamplerSettings settings) {
	_samplerSettings = settings;
}

void Sampler::Bind() {
	// Binds sampler settings to the currently bound 2D texture
	// TODO: Genericize this over other dimension'd textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _samplerSettings.TextureMapModeUGL());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, _samplerSettings.TextureMapModeUGL());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _samplerSettings.TextureMinFilterGL());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _samplerSettings.TextureMagFilterGL());
}
Sampler::Sampler(SamplerSettings settings) {
	SetSettings(settings);
}
