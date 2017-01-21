#include "includes.h"

#include "window.h"

GameWindow::GameWindow(int width,int height,const char *name,bool resizeable, bool verbose){
    IS_ATTEMPTING=false;
    VERBOSE=verbose;

    if(verbose) fprintf(stderr,"\n");

    //Create SDL Window
    if (verbose) StartAttempt("create window");
    if (SDL_Init(SDL_INIT_EVERYTHING)!=0){
        CheckError();
        return;
    }
    
    if (verbose) fprintf(stderr,"Setting OpenGL basic attributes\n");
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    
    if (verbose) fprintf(stderr,"Setting OpenGL version and profile\n");
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    CheckError();
    
    int flags=SDL_WINDOW_OPENGL;
    if(resizeable)
        flags=SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    SCREEN=SDL_CreateWindow(name,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,flags);
    if (SCREEN==NULL){
        CheckError();
        return;
    }

    GL_CONTEXT=SDL_GL_CreateContext(SCREEN);
    if (!GL_CONTEXT){
        CheckError();
        return;
    }
    
    if (verbose){
        int major,minor;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
        fprintf(stderr, "Using OpenGL version %d.%d\n",major,minor);
    }
    SCREEN_WIDTH=width;
    SCREEN_HEIGHT=height;
    aspect=((float)SCREEN_WIDTH)/SCREEN_HEIGHT;
    if (verbose) EndAttempt(); //Finished setting up window

    if (verbose) StartAttempt("set up OpenGL");
    glClearColor(0.0f,0.0f,0.4f,0.0f);
    /*SetZBuffer(zBuffer);
    SetBackFaceCull(cullBackFaces);
    SCREEN_FOV=FOV;
    ProjectionMatrix=CalculateProjectionMatrix();*/
    CheckError();
    if (verbose) EndAttempt();
}

void GameWindow::StartAttempt(std::string attemptString){
    if (!IS_ATTEMPTING){
        IS_ATTEMPTING=true;
        fprintf(stderr, "Attempting to %s...\n",attemptString.c_str());
    }
}

void GameWindow::EndAttempt(bool success){
    if (IS_ATTEMPTING){
        fprintf(stderr, "\n...%s\n", success ? "Success" : "Faliure");
        IS_ATTEMPTING=false;
    }
}
   

/*glm::mat4 GameWindow::CalculateProjectionMatrix(){
    float FOVrads=glm::radians(SCREEN_FOV),aspect=(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
    return glm::perspective(FOVrads,aspect,0.1f,100.0f);
}

void GameWindow::UseShaderProgram(GLuint program){
    glUseProgram(program);
}*/

void GameWindow::SetClearColor(float r,float g,float b,float a){
    if (VERBOSE) fprintf(stderr, "Setting clear color to (%f %f %f %f)\n",r,b,g,a);
    glClearColor(r,g,b,a);
}

void GameWindow::ClearWindow(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameWindow::Flip(void){
    SDL_GL_SwapWindow(SCREEN);
}

void GameWindow::ApplyResolution(void){
    glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
}

/*glm::mat4 GameWindow::SetFOV(float newFOV){
    if (VERBOSE) fprintf(stderr, "Setting FOV to %f degrees\n", newFOV);
    SCREEN_FOV=newFOV;
    ProjectionMatrix=CalculateProjectionMatrix();
    return ProjectionMatrix;
}

void GameWindow::SetZBuffer(bool newShouldZBuffer){
    if (newShouldZBuffer){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }else 
        glDisable(GL_DEPTH_TEST);
    Z_BUFFER_ENABLED=newShouldZBuffer;
    if (VERBOSE) fprintf(stderr, "Enable Depth Test: %s\n", Z_BUFFER_ENABLED ? "true" : "false");
}

void GameWindow::SetBackFaceCull(bool newBackFaceCull){
    if (newBackFaceCull){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }else{
        glDisable(GL_CULL_FACE);
    }
    BACK_FACE_CULL=newBackFaceCull;
    if (VERBOSE) fprintf(stderr, "Enable Backface Culling: %s\n", newBackFaceCull ? "true" : "false");
   }*/

void GameWindow::End(){
    SDL_DestroyWindow(SCREEN);
    SDL_Quit();
    return;
}
