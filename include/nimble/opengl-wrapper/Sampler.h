#pragma once

#include "nimble/utility/Macros.h"
#include <GL/glew.h>
#include <cassert>
#include <nimble/core/Assert.h>
#include <nimble/utility/StringUtility.h>

namespace Nimble {

enum TextureWrapMode {
    ClampToEdge,
    Repeat,
    MirroredRepeat
};

namespace SamplerConstants {

static const std::string ClampToEdgeStr = std::string("ClampToEdge");
static const std::string RepeatStr = std::string("Repeat");
static const std::string MirroredRepeat = std::string("MirroredRepeat");

static const std::string NearestStr = std::string("Nearest");
static const std::string LinearStr = std::string("Linear");

static const std::string NearestMipmapNearestStr = std::string("NearestMipmapNearest");
static const std::string LinearMipmapNearestStr = std::string("LinearMipmapNearest");
static const std::string NearestMipmapLinearStr = std::string("NearestMipmapLinear");
static const std::string LinearMipmapLinearStr = std::string("LinearMipmapLinear");
}

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

struct SamplerSettingsUtil {
	static TextureWrapMode TextureWrapModeFromString(const std::string &value) {
		if (iequals(value, SamplerConstants::ClampToEdgeStr)) {
			return TextureWrapMode::ClampToEdge;
		} else if (iequals(value, SamplerConstants::RepeatStr)) {
			return TextureWrapMode::Repeat;
		} else if (iequals(value, SamplerConstants::MirroredRepeat)) {
			return TextureWrapMode::MirroredRepeat;
		} else {
			ASSERT_TRUE(false, fmt::format("Texture Wrap Mode String ({}) does not map to a known enum value", value).c_str());
		}
	}

	static TextureMagFilter TextureMagFilterFromString(const std::string &value) {
		if (iequals(value, SamplerConstants::NearestStr)) {
			return TextureMagFilter::Nearest;
		} else if (iequals(value, SamplerConstants::LinearStr)) {
			return TextureMagFilter::Linear;
		} else {
			ASSERT_TRUE(false, fmt::format("Texture Mag Filter String ({}) does not map to a known enum value", value).c_str());
		}
	}

	static TextureMinFilter TextureMinFilterFromString(const std::string &value) {
		if (iequals(value, SamplerConstants::NearestStr)) {
			return TextureMinFilter::Nearest;
		} else if (iequals(value, SamplerConstants::LinearStr)) {
			return TextureMinFilter::Linear;
		} else if (iequals(value, SamplerConstants::NearestMipmapNearestStr)) {
			return TextureMinFilter::NearestMipmapNearest;
		} else if (iequals(value, SamplerConstants::LinearMipmapNearestStr)) {
			return TextureMinFilter::LinearMipmapNearest;
		} else if (iequals(value, SamplerConstants::NearestMipmapLinearStr)) {
			return TextureMinFilter::NearestMipmapLinear;
		} else if (iequals(value, SamplerConstants::LinearMipmapLinearStr)) {
			return TextureMinFilter::LinearMipmapLinear;
		} else {
			DIE("Texture Min Filter String ({}) does not map to a known enum value", value);
			// Work around for pointless warning
			return TextureMinFilter::Nearest;
		}
	}
};

struct SamplerSettings {
    TextureWrapMode textureWrapModeU = TextureWrapMode::Repeat;
    TextureWrapMode textureWrapModeV = TextureWrapMode::Repeat;
    TextureMagFilter textureMagFilter = TextureMagFilter::Linear;
    TextureMinFilter textureMinFilter = TextureMinFilter::NearestMipmapLinear;

    NIMBLE_INLINE unsigned int TextureMapModeUGL() {
        switch (textureWrapModeU) {
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
        switch (textureWrapModeV) {
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