#version 400

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColour;

in vec4 aux;

uniform vec3 transform_vector;

out vec3 colour;

void main() {
    colour = vertexColour;
    gl_Position = vec4(vertexPosition + transform_vector, 1.0);
}
