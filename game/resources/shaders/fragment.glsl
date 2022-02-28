#version 430
uniform sampler2D srcTex;
in vec2 texCoord;
out vec4 color;
void main() {
	//vec3 c = texture(srcTex, texCoord).xyz;
	color = vec4(1.0, 0.5, 0.0, 1.0);
}