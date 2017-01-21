#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 UV;
layout(location=2) in vec3 normalIN;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform vec3 inColor;

out vec2 uv;
out vec3 normal;
void main(){
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    normal=normalIN;
    uv=UV;
}
