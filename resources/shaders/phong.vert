#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in vec3 in_bitangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace;
out mat3 TBN;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    FragPos = vec3(Model * vec4(in_position, 1.0));
    Normal = transpose(inverse(mat3(Model))) * in_normal;
    TexCoord = in_uv;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = Projection * View * Model * vec4(in_position, 1.0);
    vec3 T = normalize(vec3(Model * vec4(in_tangent, 0.0)));
    vec3 B = normalize(vec3(Model * vec4(in_bitangent, 0.0)));
    vec3 N = normalize(vec3(Model * vec4(in_normal, 0.0)));
    TBN = mat3(T, B, N);
}