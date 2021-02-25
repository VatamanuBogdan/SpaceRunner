#version 330

uniform sampler2D texture_1;

in vec2 tex_coord;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture2D(texture_1, tex_coord);
}