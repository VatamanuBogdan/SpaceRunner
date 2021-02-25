#version 330 core
layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 tex_coords;

out vec2 f_tex_coords;

void main() {
    gl_Position = vec4(v_pos, 0.0, 1.0);
    f_tex_coords = tex_coords;
}