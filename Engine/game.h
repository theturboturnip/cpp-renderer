#ifndef __INC_GAME_H__
#define __INC_GAME_H__

#include "includes.h"
#include "loader.h"
#include "object.h"
#include "mesh.h"
#include "playground_parser.h"
#include "playground_scene.h"
#include "window.h"

const char* projectFolder;
int main(int argc,char* argv[]);
int LoadProject();
int LoadScene(bool createCamera=true);
void DrawFullscreenQuad(GLuint texture);
GameWindow *window=NULL;
PlaygroundScene *scene;
PlaygroundFile *config;
GLuint additiveFramebuffer,additiveTex,additiveRenderBuffer;

#endif
