#version 460 core

in vec4 vertColor;
in vec3 FragPos;

out vec4 FragColor;

#define PI 3.141592
#define N 0.2

void main() {
    // TODO: This pure-white hack is in place because of the shadow frustum debug
    // cube. The mesh loader doesn't understand a `PositionColor` mesh so vertex
    // colors are garbage. Fix the resource manager (and add the ability to
    // clone meshes so their contents can be modified) and then revert this to using
    // vertex colors
    FragColor = vertColor;
}