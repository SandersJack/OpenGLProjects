#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;


// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    UV = vertexUV;
}
