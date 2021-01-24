#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
   FragPos = vec3(View * Model * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(View * Model))) * normal;
   gl_Position = Projection * View * Model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}