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

extern const std::string ClampToEdgeStr;
extern const std::string RepeatStr;
extern const std::string MirroredRepeat;

extern const std::string NearestStr;
extern const std::string LinearStr;

extern const std::string NearestMipmapNearestStr;
extern const std::string LinearMipmapNearestStr;
extern const std::string NearestMipmapLinearStr;
extern const std::string LinearMipmapLinearStr;
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
			DIE(fmt::format("Texture Wrap Mode String ({}) does not map to a known enum value", value).c_str());
			// Silence silly warning
			return TextureWrapMode::ClampToEdge;
		}
	}

	static TextureMagFilter TextureMagFilterFromString(const std::string &value) {
		if (iequals(value, SamplerConstants::NearestStr)) {
			return TextureMagFilter::Nearest;
		} else if (iequals(value, SamplerConstants::LinearStr)) {
			return TextureMagFilter::Linear;
		} else {
			DIE(fmt::format("Texture Mag Filter String ({}) does not map to a known enum value", value).c_str());
			// Silence silly warning
			return TextureMagFilter::Linear;
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

    [[nodiscard]] NIMBLE_INLINE unsigned int TextureMapModeUGL() const {
		unsigned int result;
        switch (textureWrapModeU) {
            case TextureWrapMode::ClampToEdge:
                result = GL_CLAMP_TO_EDGE;
				break;
            case TextureWrapMode::Repeat:
                result = GL_REPEAT;
                break;
            case TextureWrapMode::MirroredRepeat:
                result = GL_MIRRORED_REPEAT;
                break;
            default:
                assert(false);
                result = -1;
        }

		return result;
    }

    [[nodiscard, maybe_unused]] NIMBLE_INLINE unsigned int TextureMapModeVGL() const {
		unsigned int result;
        switch (textureWrapModeV) {
            case TextureWrapMode::ClampToEdge:
                result = GL_CLAMP_TO_EDGE;
				break;
            case TextureWrapMode::Repeat:
                result = GL_REPEAT;
                break;
            case TextureWrapMode::MirroredRepeat:
                result = GL_MIRRORED_REPEAT;
                break;
            default:
                assert(false);
                result = -1;
        }

		return result;
    }

    [[nodiscard]] NIMBLE_INLINE unsigned int TextureMagFilterGL() const {
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

    [[nodiscard]] NIMBLE_INLINE unsigned int TextureMinFilterGL() const {
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
	explicit Sampler(SamplerSettings settings);

    void SetSettings(SamplerSettings settings);
    void Bind();
};

}