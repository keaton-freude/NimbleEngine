#version 430 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D grid_texture;

#define UV_MULT 1024

void main() {
    FragColor = texture2D(grid_texture, TexCoord * UV_MULT) * vec4(0.8f, 0.8f, 0.8f, 0.8f);
}
