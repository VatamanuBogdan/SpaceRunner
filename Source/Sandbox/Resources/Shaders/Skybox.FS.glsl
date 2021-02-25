#version 330
in vec3 tex_coord;
uniform samplerCube skybox;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture(skybox, tex_coord);
}
