#pragma once

#include <string>
#include <gl/glew.h>
#include <cassert>
#include "nimble/utility/Macros.h"

namespace Nimble {


class Texture2D {
private:
    unsigned int _texture;
    int _width;
    int _height;
    int _num_channels;

public:
    Texture2D();
    void LoadFromFile(const std::string& path);
};

}