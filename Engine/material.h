#ifndef __INC_MAT_H__
#define __INC_MAT_H__

#include "includes.h"

class Material{
public:
    Material(GLuint sID);
    bool SetFloat(const char* floatKey,float toSet);
    bool SetTexture(const char* texKey,GLuint tex);
    bool SetVector(const char* vecKey,glm::vec3 *vec);
    bool SetMatrix(const char* matKey,glm::mat4 *mat,bool modelRelative=true);
    void Apply(glm::mat4 M,glm::mat4 VP);
private:
    std::map<GLuint,float> *floats;
    std::map<GLuint,glm::vec3> *vectors;
    std::map<GLuint,GLuint> *textures;
    std::map<GLuint,glm::mat4> *matrices;
    std::map<GLuint,bool> *matModelR;
    std::map<GLuint,GLenum> *types;
    GLuint shaderProgram;
    GLuint MVPloc;
};

#include "loader.h"
#include "playground_parser.h"

extern std::map<std::string,Material*> *materials;
Material* LoadMaterial(std::string name, const char* projectFolder);

#endif
