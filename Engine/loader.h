#ifndef __INC_LOADER_H__
#define __INC_LOADER_H__

#include "includes.h"
#include "playground_parser.h"
#include "material.h"
#include "mesh.h"
#include "lodepng.h"

using namespace std;

GLuint LoadShaderFromFile(string pathString,GLenum shaderType);
GLuint LoadShadersIntoProgram(string vertexPath,string fragPath,GLuint program=0);
GLuint LoadTextureFromFile(string imagePath, GLuint imageType);
GLuint LoadBMPFromFile(string imagePath,GLuint imageType);
GLuint LoadPNGFromFile(string imagePath,GLuint imageType);
int LoadModelFromFile(string modelPath, GLuint buffers[3], float scale=1.0);
int LoadModelFromFile(string modelPath, GLuint buffers[3], glm::mat3 *transform=NULL, glm::vec3 *translation=NULL);

#endif

