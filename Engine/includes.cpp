#include "includes.h"

using namespace std;

glm::vec3* stov3(string s){
    vector<string> *vecConvUtil=new vector<string>();
    SplitString(s,',',vecConvUtil);
    glm::vec3 *toReturn=new glm::vec3(0);
    toReturn->x=stof((*vecConvUtil)[0]);
    toReturn->y=stof((*vecConvUtil)[1]);
    toReturn->z=stof((*vecConvUtil)[2]);
    return toReturn;
}

void SplitString(string& str, char delim,vector<string> *v) {
    auto i = 0;
    auto pos = str.find(delim);
    while (pos != string::npos) {
      v->push_back(str.substr(i, pos-i));
      i = ++pos;
      pos = str.find(delim, pos);

      if (pos == string::npos)
         v->push_back(str.substr(i, str.length()));
    }
}

void CheckError(const char* msg,bool checkSDL,bool checkGL){
    if(checkSDL){
        const char *error = SDL_GetError();
        if (*error != '\0'){
            //if (IS_ATTEMPTING) EndAttempt(false);
        	fprintf(stderr,"SDL Error %s: %s\n", msg, error);
        	SDL_ClearError();
        }
    }
    if(checkGL){
        GLuint GLError=glGetError();
        if (GLError!=0){
            //if (IS_ATTEMPTING) EndAttempt(false);
            fprintf(stderr,"OpenGL Error %s: %d\n", msg, GLError);
        }
    }
}
