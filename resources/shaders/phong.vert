#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 lightPos;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

void main()
{
   LightPos = vec3(View * vec4(lightPos, 1.0));
   FragPos = vec3(View * Model * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(View * Model))) * normal;
   gl_Position = Projection * View * Model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}