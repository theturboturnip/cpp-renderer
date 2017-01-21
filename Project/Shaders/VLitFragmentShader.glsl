#version 330 core
uniform vec3 inColor;

in float lightLevel;
out vec3 color;
void main(){
     color=inColor*lightLevel;//*vec3(1,0.5,0.5);
}
