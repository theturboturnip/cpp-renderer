#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 UV;
layout(location=2) in vec3 normal;

uniform mat4 MVP;

void main(){
     gl_Position=MVP*vec4(vertexPosition_modelspace.xyz,1);
}