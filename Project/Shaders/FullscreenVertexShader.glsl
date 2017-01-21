#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_fake;
layout (location = 2) in vec3 normal;

out vec2 uv;

void main(){
     gl_Position=vec4(position.x,position.y,0,1);
     uv=uv_fake;
}