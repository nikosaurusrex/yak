#version 330 core

in vec2 out_tex;

out vec4 color;

uniform sampler2D sampler;

void main() {
    color = texture(sampler, out_tex);
}