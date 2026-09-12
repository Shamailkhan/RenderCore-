#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in float thickness;

uniform mat4 u_mvp;

out VS_OUT
{
    vec4 color;
    float thickness;
} vs_out;

void main()
{
    gl_Position = u_mvp * vec4(position, 1.0);

    vs_out.color = color;
    vs_out.thickness = thickness;
}