#include "game.h"

#define NUM_KEYS 12
static int keys[NUM_KEYS] = {
SDLK_w, SDLK_s,
SDLK_a, SDLK_d,
SDLK_SPACE, SDLK_LSHIFT,
SDLK_UP, SDLK_DOWN,
SDLK_LEFT, SDLK_RIGHT,
SDLK_TAB, SDLK_LALT
};

static int keys_down_only[NUM_KEYS] = {
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    0, 0,
    1, 0
};

int LoadProject(){
    //Create GameWindow
    //read config.cfg in projectFolder
    char configFileLoc[256];
    sprintf(configFileLoc,"%s/config.cfg",projectFolder);
    fprintf(stderr,"\nLooking for scene config at %s\n",configFileLoc);
    std::ifstream configFile(configFileLoc);
    if (configFile==NULL){
        fprintf(stderr,"No config file found in project.\n");
        return 0;
    }

    int width=640,height=480;
    bool resizeable=true;
    const char* name="Window";
    config=new PlaygroundFile(&configFile);

    std::string newName=config->IdentifyValue("WindowName");
    if(newName.compare("")!=0)
        name=newName.c_str();
    std::string newWidth=config->IdentifyValue("WindowWidth");
    if (newWidth.compare("")!=0)
        width=std::stoi(newWidth);
    std::string newHeight=config->IdentifyValue("WindowHeight");
    if (newHeight.compare("")!=0)
        height=std::stoi(newHeight);
    
    window=new GameWindow(width,height,name,resizeable);

    return LoadScene();
}

int LoadScene(bool sceneOnly){
    //Find scene
    std::string sceneName=config->IdentifyValue("StartScene");
    if(sceneName.compare("")==0){
        fprintf(stderr,"No starting scene was found so no objects will be loaded.\n");
        return 1;
    }
    char sceneFileLoc[256];
    sprintf(sceneFileLoc,"%s/Scenes/%s.scene",projectFolder,sceneName.c_str());
    fprintf(stderr,"\nLooking for scene file at %s\n",sceneFileLoc);
    scene=new PlaygroundScene(sceneFileLoc);
    fprintf (stderr,"Scene file parsed!\n");

    //Load Objects
    scene->IdentifyObjects(projectFolder,sceneOnly);
    scene->camera->SetAspectRatio(window->aspect);
    scene->camera->CalculateVP();
    fprintf(stderr,"Touched\n");

    //delete config;
    //delete configFile;

    return 1;
}


void InitGraphics(){
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
}

void DrawFullscreenQuad(GLuint texture){
    //fullscreenQuad,fullscreenMat
    glm::mat4 identity=glm::mat4();
    //glViewport(0,0,1,1);
    bool foundTex=scene->fullscreenMat->SetTexture("screenTexture",texture);
    if (!foundTex){
        fprintf(stderr,"Didn't find the texture\n");
    }
    scene->fullscreenMat->Apply(identity,identity);
    //fprintf(stderr,"vertex count: %d\n",scene->fullscreenQuad->vertexNum);
    scene->fullscreenQuad->Draw();
}


void RenderingPass(ShadowLight *l,glm::mat4 *VP,bool additive=false){
    if(additive){
        glBindFramebuffer(GL_FRAMEBUFFER,additiveFramebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, additiveTex, 0); 
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, additiveRenderBuffer); 
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
    }else{
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClear(GL_DEPTH_BUFFER_BIT);

    }
    for(uint i=0;i<scene->objects->size();i++){
        Material *m=(*scene->objects)[i].mat;
        if(l!=NULL)
            l->SetupMaterial(m);
        (*scene->objects)[i].Draw(VP);
        if(l!=NULL)
            l->ResetMaterial(m);
    }
    if (additive){
        glBlendFunc(GL_ONE,GL_ONE);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        DrawFullscreenQuad(additiveTex);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glClearColor(0,0,0.4f,0);
    }else
        glBlendFunc(GL_ONE,GL_ZERO);
}

int RenderScene(glm::mat4 *VP){
    window->ClearWindow();
    uint i,j,sLightCount;
    ShadowLight *l;
    
    sLightCount=scene->sLights->size();

    for(j=0;j<sLightCount;j++){
        
        l=((*scene->sLights)[j]);
        l->FindVP();
        VP=&(l->VP);
        l->InitShadowRender();
        for(i=0;i<scene->objects->size();i++){
            (*scene->objects)[i].Draw(VP,&(*scene->shadowMat));
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    window->ApplyResolution();
    window->ClearWindow();

    VP=&(scene->camera->VP);
    
    //Simple multi-light rendering. Do one base pass, then 1x additive pass per light.
    //the additive pass uses a secondary framebuffer, and then that buffer is drawn onto the screen
    
    //bool additive=false;
    
    //glBindFramebuffer(GL_FRAMEBUFFER,additiveFramebuffer);
    
    for(i=0;i<sLightCount;i++){
        RenderingPass((*scene->sLights)[i],VP,(i>0));
        //additive=true;
    }
    glBlendFunc(GL_ONE,GL_ZERO);//Make sure it works for the shadow pass

    window->Flip();
    return 1;
}

void HandleInput(int *keys_down,float deltaTime){
    //Camera translation
    float cameraHorizMoveSpeed=2.0f,cameraVertMoveSpeed=1.0f;
    glm::vec3 *moveDir=(new glm::vec3(0));
    if (keys_down[0]) moveDir->z=1;
    if (keys_down[1]) moveDir->z=-1;
    if (keys_down[2]) moveDir->x=1;
    if (keys_down[3]) moveDir->x=-1;
    *moveDir=(*moveDir*cameraHorizMoveSpeed);
    if (keys_down[4]) moveDir->y=-cameraVertMoveSpeed;
    if (keys_down[5]) moveDir->y=cameraVertMoveSpeed;
    //glm::rotateY(*moveDir,glm::radians(scene->camera->t->rotation.y));
    *moveDir=glm::vec3((scene->camera->t->GetRotationMatrix())*glm::vec4(*moveDir,0));
    *moveDir=(*moveDir*deltaTime);
    (scene->camera->t->position)=(scene->camera->t->position)+*moveDir;

    //Camera rotation
    float cameraRotateSensitivity=30.0f;
    glm::vec3 *deltaLook=new glm::vec3(0);
    if (keys_down[6]) deltaLook->x=+1;
    if (keys_down[7]) deltaLook->x=-1;
    if (keys_down[8]) deltaLook->y=+1;
    if (keys_down[9]) deltaLook->y=-1;
    *deltaLook=*deltaLook*cameraRotateSensitivity*deltaTime;
    (scene->camera->t->rotation)=scene->camera->t->rotation+*deltaLook;

    if (keys_down[10]&&!keys_down[11]){
        fprintf(stderr,"Reset Scene\n");
        LoadScene(false);
    }
    scene->camera->CalculateVP();

    delete moveDir;
    delete deltaLook;
}

void GameLoop(){
    bool shouldEnd=false;
    SDL_Event e;
    glm::mat4 *VP;
    //fprintf(stderr,"Game loop\n");
    int *keys_down=(int*)calloc(NUM_KEYS,sizeof(int));
    uint i;
    float deltaTime=1/60.f,startTime;
    
    //GLuint additiveFramebuffer,additiveTexture;
    glGenFramebuffers(1,&additiveFramebuffer);

    glGenTextures(1, &additiveTex);
    glBindTexture(GL_TEXTURE_2D, additiveTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->SCREEN_WIDTH, window->SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, additiveTex, 0);  

    //GLuint additiveRenderBuffer;
    glGenRenderbuffers(1, &additiveRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, additiveRenderBuffer); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window->SCREEN_WIDTH, window->SCREEN_HEIGHT);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, additiveRenderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fprintf(stderr,"Framebuffer not complete\n");
        return;
    }
    
    while(!shouldEnd){
        startTime=SDL_GetTicks();
        
        for (i=0;i<NUM_KEYS;i++){
            if (keys_down_only[i]&&keys_down[i]){
                keys_down[i]=0;
            }
        }

        while (SDL_PollEvent(&e)!=0){
            if (e.type==SDL_QUIT)
                shouldEnd=true;
            else if (e.type==SDL_KEYDOWN) {
                for (i=0; i<sizeof(keys)/sizeof(e.key.keysym.sym); i++) {
                    if (e.key.keysym.sym==keys[i]) keys_down[i]=1;
                }
            }
            else if (e.type==SDL_KEYUP) {
                for (i=0; i<sizeof(keys)/sizeof(e.key.keysym.sym); i++) {
                    if (e.key.keysym.sym==keys[i]) keys_down[i]=0;
                }
            }
        }
        HandleInput(keys_down,deltaTime);
        #pragma GCC diagnostic ignored "-Wmaybe-uninitialized" 
        RenderScene(VP);//VP isn't meant to be initialized, I just don't want to create a new matrix each tick
        deltaTime=(SDL_GetTicks()-startTime)/1000;
    }

    delete keys_down;
    delete VP;
}

int main(int argc,char* argv[]){
    if (argc<2){
        fprintf(stderr, "No project folder found.\n");
        return 0;
    }
    projectFolder=argv[1];
    if (LoadProject()==0)
        return 0;
    InitGraphics();

    GameLoop();
    
    window->End();
    return 1;
}
