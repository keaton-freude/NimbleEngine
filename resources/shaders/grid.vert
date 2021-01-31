#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uv;

out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
    FragPos = vec3(View * Model * vec4(in_position, 1.0));
    gl_Position = Projection * View * Model * vec4(in_position.xyz, 1.0);
    TexCoord = in_uv;
}
