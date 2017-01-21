#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 UV;
layout(location=2) in vec3 normalIN;

// Values that stay constant for the whole mesh.
uniform mat4 M;
uniform mat4 VP;
uniform vec3 inColor;
uniform mat4 SLightMVP;

out vec4 shadowmapPos;
out vec2 uv;
out vec3 normal;
out vec3 worldPos;

void main(){
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  VP * M * vec4(vertexPosition_modelspace,1);
    worldPos=vec3(gl_Position);
    shadowmapPos= (SLightMVP*vec4(vertexPosition_modelspace,1));
    uv=UV;
    normal=vec3(M*vec4(normalIN,0));
}
