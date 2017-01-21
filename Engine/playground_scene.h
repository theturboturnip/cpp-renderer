#ifndef __INC_PLAY_SCENE_H__
#define __INC_PLAY_SCENE_H__

#include "includes.h"
#include "material.h"
#include "mesh.h"
#include "object.h"

using namespace std;

class PlaygroundScene : public PlaygroundFile{
public:
    PlaygroundScene(const char* filename);
    PlaygroundScene(ifstream *file);
    void IdentifyObjects(const char* projectFolder,bool createCamera=true,Camera* replacementCamera=NULL); 
//glm::vec3* stov3(string s);
    vector<Object> *objects;
    vector<ShadowLight*> *sLights;
    Material *shadowMat,*fullscreenMat;
    Mesh *fullscreenQuad;
    Camera *camera;
};

#endif
