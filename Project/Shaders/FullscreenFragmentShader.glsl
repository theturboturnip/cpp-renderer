#version 330
in vec2 uv;
out vec4 color;

uniform sampler2D screenTexture;

void main(){
    color=texture(screenTexture,uv);
}
