#include "nimble/opengl-wrapper/Texture2D.h"
#include <GL/glew.h>
#include "stb_image.h"
#include <cassert>
#include "spdlog/spdlog.h"

using namespace Nimble;

Texture2D::Texture2D() {
    glGenTextures(1, &_texture);
}

void Texture2D::LoadFromFile(const std::string& path) {
    unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &_num_channels, 0);
    assert(data);

    glBindTexture(GL_TEXTURE_2D, _texture);

    switch (_num_channels) {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R, _width, _height, 0, GL_R, GL_UNSIGNED_BYTE, data);
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, _width, _height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            spdlog::critical("Texture at path {} contains an unexpected amount of channels: {}", path.c_str(), _num_channels);
            break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}