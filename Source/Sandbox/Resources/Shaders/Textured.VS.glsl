#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 ViewPosition;

out vec3 f_position;
out vec3 f_normal;
out vec2 tex_coord;
out vec3 view_dir;


void main() {
    f_position = (Model * vec4(v_position, 1.0f)).xyz;
    f_normal = normalize(mat3(Model) * v_normal);
    tex_coord = v_texture_coord;
    view_dir = normalize(ViewPosition - f_position);
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
