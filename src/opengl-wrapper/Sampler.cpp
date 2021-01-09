#include "nimble/opengl-wrapper/Sampler.h"

using namespace Nimble;

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