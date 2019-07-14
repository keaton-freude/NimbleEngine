#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
uniform vec3 test;

out vec4 vertColor;

void main()
{
    vertColor = vec4(test.x, test.y, test.z, 1.0);
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}