#version 330 core

layout(location=0) out vec4 out_color;
layout(location=1) out int out_entity;

uniform vec4 in_color;
uniform int in_entity;

void main() {
    out_color = in_color;
    out_entity = in_entity;
}
