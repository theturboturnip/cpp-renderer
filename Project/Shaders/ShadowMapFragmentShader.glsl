#version 330 core

uniform vec3 DiffuseTint;
uniform sampler2D DiffuseTex;

uniform sampler2D SLightDepthMap;
uniform vec3 SLightColor;
uniform float SLightSpotAttenStart;
uniform float SLightNearClip,SLightFarClip;
uniform float SLightDistAttenStart;
uniform vec3 SLightDir;

in vec4 shadowmapPos;
in vec2 uv;
in vec3 normal;
in vec3 worldPos;

out vec3 color;

float pow5(float x){
    return x*x*x*x*x;
}

float InterpretShadowmap(float shadowmapValue){
    //Gets z-value from shadowmap 
    return (2 * SLightNearClip) / (SLightFarClip + SLightNearClip - shadowmapValue * (SLightFarClip - SLightNearClip));
}

float LightLevel(){
    float shadowmapPosX=shadowmapPos.x/shadowmapPos.w/2.0f+0.5f;
    float shadowmapPosY=shadowmapPos.y/shadowmapPos.w/2.0f+0.5f;
    vec2 newUV=vec2(shadowmapPosX,shadowmapPosY);
    float renderedZ=texture2D(SLightDepthMap,newUV).r;
    renderedZ=InterpretShadowmap(renderedZ); //Get the z-value from the shadowmap
    float currentZ=shadowmapPos.z/shadowmapPos.w;
    currentZ=InterpretShadowmap(currentZ); //Get the pixel z-value
    //
    float lightLevel=clamp(dot(normal/length(normal),SLightDir),0,1),bias=0.005f; //Lambert NdotL, inaccuracy bias
    if(renderedZ<currentZ+bias)
        lightLevel=0;
    //If blurStart<1 it isn't a spotlight
    //Else use spotlight attenuation
    //if d=1 a=0
    //if d<=bs a=1
    //else a=1-max(d-bs,0)/(1-bs)
    //if bs>1.5 a=1 no matter what
    float d=length(shadowmapPos.xy/shadowmapPos.w); //Get Length from centre
    float atten=1-(max(d-SLightSpotAttenStart,0)/(1.001-SLightSpotAttenStart));
    return lightLevel*atten;
}

void main(){
    vec3 inColor=DiffuseTint*texture2D(DiffuseTex,uv).rgb;
    color=SLightColor*inColor*LightLevel();

    //color=texture2D(SLightDepthMap,uv).rgb;
}
