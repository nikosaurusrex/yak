#version 330 core

in vec2 out_tex;

out vec4 color;

uniform vec4 in_color;
uniform sampler2D sampler;

void main() {
    color = in_color * texture(sampler, out_tex);
}
