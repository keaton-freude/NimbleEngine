#version 460 core

in vec4 vertColor;
in vec3 FragPos;

out vec4 FragColor;

#define PI 3.141592
#define N 0.2

void main()
{
    FragColor = vertColor;
}