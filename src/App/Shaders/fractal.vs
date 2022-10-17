#version 330 core

layout(location=0) in vec2 pos;

out vec2 vert_pos;

uniform mat4 transform;

void main() {
    vec4 calc_pos = transform * vec4(pos.xy, 0.0, 1.0);
    vert_pos = calc_pos.xy;
	gl_Position = vec4(pos.xy, 0.0, 1.0);
}
