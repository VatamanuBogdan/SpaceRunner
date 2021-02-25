#version 330
layout (location = 0) in vec3 v_pos;

uniform vec2 Position;
uniform float xScale;
uniform float yScale;

void main() {
    gl_Position = vec4(Position.x + xScale * v_pos.x, Position.y + yScale * v_pos.y, 0.0f, 1.0f);
}
