#version 330
out vec3 color;
in vec2 uv;
uniform sampler2D SLightDepthMap;

void main(){
     color=vec3(texture2D(SLightDepthMap,uv).r);
     color=vec3(color.r)/2;
}
