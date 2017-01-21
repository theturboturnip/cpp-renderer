#include "object.h"

Object::Object(glm::vec3 *pos,glm::vec3 *rot,glm::vec3 *scale,Mesh *m,Material *material,const char* _type,std::map<std::string,std::string> *_data){
    if(scale==NULL)
        scale=new glm::vec3(1);
    t=new Transform(pos,rot,scale);
    mesh=m;
    mat=material;
    type=_type;
    data=_data;
}

void Object::Draw(glm::mat4 *VP){
    if (mesh==NULL||mat==NULL)
        return;
    //glm::mat4 MVP=(*VP)*(t->Evaluate());
    mat->Apply(t->Evaluate(),*VP);
    mesh->Draw();
}

void Object::Draw(glm::mat4 *VP,Material *overrideMat){
    if (mesh==NULL)
        return;
    overrideMat->Apply(t->Evaluate(),*VP);
    mesh->Draw();
}

Camera::Camera(glm::vec3 *pos,glm::vec3 *rot,glm::vec3 *scale,const char* _type,std::map<std::string,std::string> *_data) : Object(pos,rot,scale,NULL,NULL,_type,_data){
    FOV=std::stof((*data)["FOV"]);
    nearClip=std::stof((*data)["NearClip"]);
    farClip=std::stof((*data)["FarClip"]);
}

void Camera::SetAspectRatio(float aspectRatio){
    projectionMatrix=glm::perspective(glm::radians(FOV),aspectRatio,nearClip,farClip);
    //projectionMatrix=&();
}

glm::mat4 Camera::CalculateVP(){
    VP=(projectionMatrix)*glm::inverse((t->Evaluate()));
    return VP;
}

/*void Light::Light(glm::vec3 *pos,glm::vec3 *rot,glm::vec3 *scale,const char* _type,std::map<std::string,std::string> *_data) : Camera(pos,rot,scale,_type,_data){
    Set
    }*/

ShadowLight::ShadowLight(glm::vec3 *pos,glm::vec3 *rot,const char* _type,std::map<std::string,std::string> *_data) : Object(pos,rot,NULL,NULL,NULL,_type,_data){
    resX=std::stoi((*data)["ResolutionX"]);
    resY=std::stoi((*data)["ResolutionY"]);
    color=*stov3((*data)["LightColor"]);
    //FindP();
}
void ShadowLight::SetupTexture(){
    //Gen rendering resources
    glGenFramebuffers(1,&depthMapFBO);
    
    glGenTextures(1,&depthMapTex); //Create a texture
    glBindTexture(GL_TEXTURE_2D,depthMapTex); //Set texture type
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT, //Tell OpenGL we only use depth
                 resX,
                 resY,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
    float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

void ShadowLight::InitShadowRender(){
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO); //Tell OpenGL to render to the depth map from now on
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthMapTex,0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,resX,resY);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr,"The frame buffer is not working\n");
    }
}

void ShadowLight::FindP(/*int resX,int resY*/){
    P=glm::perspective(glm::radians(110.0f),((float)resX)/resY,0.3f,50.0f);
}

void ShadowLight::SaveTexture() {
    glBindTexture(GL_TEXTURE_2D,depthMapTex); //Set texture type
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    float *raw_img = (float*) malloc(resX * resY * sizeof(float));
    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, raw_img);
    SDL_Surface *image=SDL_CreateRGBSurface(0,resX,resY,32,0,0,0,0);
    unsigned char* image_pixels=(unsigned char*)image->pixels;
    for(uint i=0;i<resX;i++){
        for(uint j=0;j<resY;j++){
            image_pixels[4*(j*resX+i)+0]=255*raw_img[j*resX+i];            
            image_pixels[4*(j*resX+i)+1]=255*raw_img[j*resX+i];            
            image_pixels[4*(j*resX+i)+2]=255*raw_img[j*resX+i];            
            image_pixels[4*(j*resX+i)+3]=255*raw_img[j*resX+i];
        }
    }
    SDL_SaveBMP(image,"depth.bmp");
    free(image);
    free(raw_img);
    //exit(4);
}

glm::mat4 ShadowLight::FindVP(){
    VP=P*glm::inverse(t->Evaluate());
    return VP;
}

void ShadowLight::SetupMaterial(Material *mat){
    if (mat==NULL) return;
    mat->SetMatrix("SLightMVP",&VP);
    mat->SetTexture("SLightDepthMap",depthMapTex);
    mat->SetVector("SLightColor",&color);
}    

void ShadowLight::ResetMaterial(Material *mat){
    //Nothing happens here as all the parameters set in SetupMaterial
    //have to be changed for the next light anyway
}

SpotLight::SpotLight(glm::vec3 *pos,glm::vec3 *rot, const char* _type,std::map<std::string,std::string> *_data) : ShadowLight(pos,rot,_type,_data){
    FindP();
    SetupTexture();
    blurStart=std::stof((*data)["AttenStart"]);
    if(blurStart>1||blurStart<0)
        blurStart=1.5f; //We assume that if it's >1 or <0 we don't want attenuation
}

void SpotLight::FindP(){
    FOV=std::stof((*data)["FOV"]);
    nearClip=std::stof((*data)["NearClip"]);
    farClip=std::stof((*data)["FarClip"]);
    P=glm::perspective(glm::radians(FOV),((float)resX)/resY,nearClip,farClip);
}

void SpotLight::SetupMaterial(Material *mat){
    if (mat==NULL) return;
    mat->SetMatrix("SLightMVP",&VP);
    mat->SetTexture("SLightDepthMap",depthMapTex);
    mat->SetVector("SLightColor",&color);
    if (mat->SetFloat("SLightNearClip",nearClip))
        mat->SetFloat("SLightFarClip",farClip);
    mat->SetFloat("SLightSpotAttenStart",blurStart);
    glm::vec3 forward=glm::vec3(t->GetRotationMatrix()*(glm::vec4(0,0,1,0)));
    mat->SetVector("SLightDir",&forward);
}

void SpotLight::ResetMaterial(Material *mat){
    if(mat==NULL) return;
    mat->SetFloat("SLightSpotAttenStart",2); //Because of the equation used for spotlight attenuation any value > root2 means no attenuation.
}
