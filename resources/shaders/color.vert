#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 FragPos;
out vec4 vertColor;

void main()
{
    FragPos = vec3(View * Model * vec4(aPos, 1.0));
    vertColor = aColor;
    gl_Position = Projection * View * Model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}