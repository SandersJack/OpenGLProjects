#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 model; // Model matrix for transformations

void main() {
    gl_Position = model * vec4(position, 0.0, 1.0);
}