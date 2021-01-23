#pragma once

#include "nimble/utility/Macros.h"
#include <gl/glew.h>
#include <cassert>

namespace Nimble {

enum TextureWrapMode {
    ClampToEdge,
    Repeat,
    MirroredRepeat
};

enum class TextureMagFilter {
    Nearest,
    Linear
};

enum class TextureMinFilter {
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

struct SamplerSettings {
    TextureWrapMode textureMapModeU = TextureWrapMode::Repeat;
    TextureWrapMode textureMapModeV = TextureWrapMode::Repeat;
    TextureMagFilter textureMagFilter = TextureMagFilter::Linear;
    TextureMinFilter textureMinFilter = TextureMinFilter::NearestMipmapLinear;

    NIMBLE_INLINE unsigned int TextureMapModeUGL() {
        switch (textureMapModeU) {
            case TextureWrapMode::ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case TextureWrapMode::Repeat:
                return GL_REPEAT;
                break;
            case TextureWrapMode::MirroredRepeat:
                return GL_MIRRORED_REPEAT;
                break;
            default:
                assert(false);
                return -1;
        }
    }

    NIMBLE_INLINE unsigned int TextureMapModeVGL() {
        switch (textureMapModeV) {
            case TextureWrapMode::ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case TextureWrapMode::Repeat:
                return GL_REPEAT;
                break;
            case TextureWrapMode::MirroredRepeat:
                return GL_MIRRORED_REPEAT;
                break;
            default:
                assert(false);
                return -1;
        }
    }

    NIMBLE_INLINE unsigned int TextureMagFilterGL() {
        switch (textureMagFilter) {
            case TextureMagFilter::Linear:
                return GL_LINEAR;
            case TextureMagFilter::Nearest:
                return GL_NEAREST;
            default:
                assert(false);
                return -1;
        }
    }

    NIMBLE_INLINE unsigned int TextureMinFilterGL() {
        switch (textureMinFilter) {
            case TextureMinFilter::Linear:
                return GL_LINEAR;
            case TextureMinFilter::Nearest:
                return GL_NEAREST;
            case TextureMinFilter::LinearMipmapLinear:
                return GL_LINEAR_MIPMAP_LINEAR;
            case TextureMinFilter::LinearMipmapNearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case TextureMinFilter::NearestMipmapLinear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case TextureMinFilter::NearestMipmapNearest:
                return GL_NEAREST_MIPMAP_NEAREST;
            default:
                assert(false);
                return -1;
        }
    }
};

class Sampler {
private:
    SamplerSettings _samplerSettings = {};
public:
    Sampler() = default;

    void SetSettings(SamplerSettings settings);
    void Bind();
};

}