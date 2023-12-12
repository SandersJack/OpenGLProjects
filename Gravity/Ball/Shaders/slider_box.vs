#version 330 core

layout(location = 0) in vec3 position;

uniform vec3 shapePosition;
uniform mat4 modelViewProjection;
uniform vec4 color;

out vec4 fragColor;

void main() {
    gl_Position = modelViewProjection * vec4(position + shapePosition, 1.0);
    fragColor = color;
}  