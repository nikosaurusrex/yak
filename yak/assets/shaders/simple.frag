#version 330 core

out vec4 out_color;
out int out_entity;

uniform vec4 in_color;
uniform int in_entity;

void main() {
    out_color = in_color;
    out_entity = in_entity;
}
