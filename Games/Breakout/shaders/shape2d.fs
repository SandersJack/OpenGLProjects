#version 330 core

out vec4 fragColor;

uniform vec3 color; // Color uniform

void main() {
    fragColor = vec4(color, 1.0);
}