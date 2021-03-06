#version 330 core

in vec2 f_tex_coords;
uniform sampler2D tex;

layout (location = 0) out vec4 out_color;

const float offset = 1.0 / 350.0f;

void main() {
    float kernel[81] = float[](
        0, 0.000001, 0.000014, 0.000055, 0.000088, 0.000055, 0.000014, 0.000001, 0,
        0.000001, 0.000036, 0.000362, 0.001445, 0.002289, 0.001445, 0.000362, 0.000036, 0.000001,
        0.000014, 0.000362, 0.003672, 0.014648, 0.023205, 0.014648, 0.003672, 0.000362, 0.000014,
        0.000055, 0.001445, 0.014648, 0.058434, 0.092566, 0.058434, 0.014648, 0.001445, 0.000055,
        0.000088, 0.002289, 0.023205, 0.092566, 0.146634, 0.092566, 0.023205, 0.002289, 0.000088,
        0.000055, 0.001445, 0.014648, 0.058434, 0.092566, 0.058434, 0.014648, 0.001445, 0.000055,
        0.000014, 0.000362, 0.003672, 0.014648, 0.023205, 0.014648, 0.003672, 0.000362, 0.000014,
        0.000001, 0.000036, 0.000362, 0.001445, 0.002289, 0.001445, 0.000362,0.000036, 0.000001,
        0, 0.000001, 0.000014, 0.000055, 0.000088, 0.000055, 0.000014, 0.000001, 0
    );

    vec3 color = vec3(0.0f);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            float s = offset * (i / 5 - 1) + f_tex_coords.s;
            float t = offset * (j / 5 - 1) + f_tex_coords.t;
            color += kernel[9 * i + j] * vec3(texture(tex, vec2(s, t)));
        }
    }
    out_color = vec4(color, 1.0);
}