#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 Color;

void main()
{    
    color = vec4(Color) * texture(image, TexCoords);
}  