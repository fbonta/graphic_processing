#version 410 core

in vec3 colour;
out vec4 frag_color;

void main() {
	frag_color = vec4 (colour, 1.0);
}