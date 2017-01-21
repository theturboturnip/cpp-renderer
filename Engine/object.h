#ifndef __INC_OBJECT_H__
#define __INC_OBJECT_H__

#include "includes.h"
#include "mesh.h"
#include "material.h"
#include "transform.h"

class Object{
public:
    Object(glm::vec3 *pos,glm::vec3 *rot,glm::vec3 *scale=NULL,Mesh *m=NULL,Material *material=NULL,const char* _type="",std::map<std::string,std::string> *_data=NULL);
    void Draw(glm::mat4 *VP);
    void Draw(glm::mat4 *VP,Material *overrideMat);
    Transform *t;
    Mesh *mesh;
    Material *mat;
    const char *type;
    std::map<std::string,std::string> *data;
};

class Camera : public Object{
public:
    Camera(glm::vec3 *pos,glm::vec3 *rot,glm::vec3 *scale,const char* _type="Camera",std::map<std::string,std::string> *_data=NULL);
    glm::mat4 projectionMatrix,VP;
    glm::mat4 CalculateVP();
    void SetAspectRatio(float aspectRatio);
    float FOV,nearClip,farClip;
};

class ShadowLight : public Object{
public:
    ShadowLight(glm::vec3 *pos,glm::vec3 *rot,const char* _type="SLight",std::map<std::string,std::string> *_data=NULL);
    virtual void FindP();
    glm::mat4 FindVP();
    void SaveTexture();
    virtual void InitShadowRender();
    glm::mat4 P;
    GLuint depthMapFBO,depthMapTex;
    uint resX,resY;
    glm::vec3 color;
    glm::mat4 VP;
    virtual void SetupMaterial(Material *mat);
    virtual void ResetMaterial(Material *mat);
    virtual void SetupTexture();
};

class SpotLight : public ShadowLight{
public:
    SpotLight(glm::vec3 *pos,glm::vec3 *rot,const char* _type="SpotLight",std::map<std::string,std::string> *_data=NULL);
    void FindP();
    void SetupMaterial(Material *mat);
    void ResetMaterial(Material *mat);
    float FOV,nearClip,farClip,blurStart;
};
    
#endif
