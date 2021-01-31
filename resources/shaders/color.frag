#version 460 core

in vec4 vertColor;
in vec3 FragPos;

out vec4 FragColor;

#define PI 3.141592
#define N 0.2

void main()
{
    vec2 Coord = FragPos.xy;
    Coord = cos(PI/N * Coord);

    //FragColor = vec4(1.0, 1.0, 1.0, 1.0) - 0.5 * smoothstep(0.98, 1.0, max(Coord.x, Coord.y));
    FragColor = vec4(1.0, 1.0, 1.0, 1.0) - 0.5 * step(0.98, max(Coord.x, Coord.y));
    FragColor.a = smoothstep(0.98, 1.0, max(Coord.x, Coord.y));
}