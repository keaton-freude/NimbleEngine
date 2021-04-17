#version 330
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    FragPos = vec3(Model * vec4(in_position, 1.0));
    Normal = mat3(transpose(inverse(Model))) * in_normal;
    TexCoord = in_uv;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = Projection * View * vec4(FragPos, 1.0);
}