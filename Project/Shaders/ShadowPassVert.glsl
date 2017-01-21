#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 UV;
layout(location=2) in vec3 normal;

uniform mat4 M
uniform mat4 VP;

void main(){
     gl_Position=VP*M*vec4(vertexPosition_modelspace,1);
}
