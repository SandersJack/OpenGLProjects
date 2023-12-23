#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform vec2 shapePosition;
uniform mat4 modelViewProjection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = modelViewProjection * vec4(shapePosition + vertex.xy, 0.0, 1.0);
}