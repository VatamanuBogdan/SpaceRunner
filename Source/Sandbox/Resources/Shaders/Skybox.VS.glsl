#version 330
layout (location = 0) in vec3 v_pos;

uniform mat4 View;
uniform mat4 Projection;

out vec3 tex_coord;

void main() {
    tex_coord = v_pos;
    gl_Position = Projection * View * vec4(v_pos, 1.0f);
}
