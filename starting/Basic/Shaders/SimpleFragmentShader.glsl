#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;


// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;


void main(){
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices
    color = texture(myTextureSampler, UV).rgb;
}