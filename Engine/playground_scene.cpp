#include "playground_scene.h"

//These don't need any code because everything's done in the PlaygroundFile initializer
PlaygroundScene::PlaygroundScene(const char* filename) : PlaygroundFile(filename){}
PlaygroundScene::PlaygroundScene(ifstream *file) : PlaygroundFile(file){}

void PlaygroundScene::IdentifyObjects(const char *projectFolder,bool createCamera,Camera* replacementCamera){
    //Identify all objects located in our file
    string line,key,value;
    bool has_object=false;
    glm::vec3 *pos,*rot,*scale;
    Material *mat;
    Mesh *mesh;
    Object *o;
    string type;

    objects=new vector<Object>();
    sLights=new vector<ShadowLight*>();
    camera=replacementCamera;
    map<string,string> *data;
    for(uint i=0;i<lines->size();i++){
        line=(*lines)[i];
        if (line.compare("OBJECT_START")==0){
            has_object=true;
            pos=new glm::vec3(0);
            rot=new glm::vec3(0);
            scale=new glm::vec3(1,1,1);
            data=new map<string,string>();
            mat=NULL;
            mesh=NULL;
            type="";
            fprintf(stderr,"\nLoading new object\n");
        }else if (line.compare("OBJECT_END")==0&&has_object){
            has_object=false;
            //Add all known data to the object
            if (type.compare("Camera")==0){
                camera=new Camera(pos,rot,scale,type.c_str(),data);
            }else if (type.compare("SpotLight")==0){
                fprintf(stderr,"Found spotlight!\n");
                SpotLight *l=new SpotLight(pos,rot,type.c_str(),data);
                sLights->push_back(l);
            }else{
                o=new Object(pos,rot,scale,mesh,mat,type.c_str(),data);
                //Push object onto list
                objects->push_back(*o);
            }
        }else if (has_object){
            //Use this line to determine more data on our object
            key=(*keys)[i];
            value=(*values)[i];
            if (key.compare("Pos")==0){
                pos=stov3(value);
                fprintf(stderr,"Pos: %s\n",value.c_str());
            }else if (key.compare("Rot")==0){
                rot=stov3(value);
                fprintf(stderr,"Rot: %f,%f,%f\n",rot->x,rot->y,rot->z);
            }else if (key.compare("Scale")==0){
                scale=stov3(value);
                fprintf(stderr,"Scale: %s\n",value.c_str());
            }else if (key.compare("Mesh")==0){
                mesh=LoadMesh(value.c_str(),projectFolder);
            }else if (key.compare("Material")==0){
                mat=LoadMaterial(value,projectFolder);
            }else if (key.compare("Type")==0){
                type=value;
            }else{
                fprintf(stderr,"Key %s Value %s\n",key.c_str(),value.c_str());
                data->emplace(key,value);
            }
        }else{
            key=(*keys)[i];
            value=(*values)[i];
            if (key.compare("ShadowMaterial")==0){
                shadowMat=LoadMaterial(value.c_str(),projectFolder);
            }else if (key.compare("FullscreenMaterial")==0){
                fullscreenMat=LoadMaterial(value.c_str(),projectFolder);
            }else if (key.compare("FullscreenQuad")==0){
                fprintf(stderr,"Found fullscreen quad %s\n",value.c_str());
                fullscreenQuad=LoadMesh(value.c_str(),projectFolder);
            }
        }
    }
    if (has_object)
        fprintf(stderr,"Object leak detected in scene\n");
}

