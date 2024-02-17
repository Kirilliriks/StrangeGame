#version 430

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec4 a_color;

uniform mat4 projection_view;
//uniform mat4 model;

void main() {;
    a_color = vec4(color, 1.0);
	gl_Position = projection_view * vec4(pos, 1.0);
}