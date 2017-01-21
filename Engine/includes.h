#ifndef __INCLUDED_INCLUDES__
#define __INCLUDED_INCLUDES__

//Standard Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include <typeinfo>
#include <utility>

//Graphics Libraries
#define GL_GLEXT_PROTOTYPES
#define GLM_FORCE_RADIANS
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL2/SDL_opengl.h>
#include <SDL_image.h>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

//Global functions
glm::vec3 *stov3(std::string s);
void SplitString(std::string &str,char delim,std::vector<std::string> *v);
void CheckError(const char* msg="",bool checkSDL=true,bool checkGL=true);

#endif
