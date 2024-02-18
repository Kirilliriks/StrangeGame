#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;

out vec4 a_color;

uniform mat4 projection_view;
uniform mat4 model;

void main() {;
    a_color = color;
	gl_Position = projection_view * model * vec4(pos, 1.0);
}