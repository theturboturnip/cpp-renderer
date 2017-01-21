#include "loader.h"
#include "lodepng.h"

using namespace std;

//All importing functions give a GLuint for referencing the loaded object.
//If the import fails, it will return 0.
//When using texture loaded using LoadImageFromFile it will be vertically flipped. Combat this by flipping y texture coordinates when importing model.

/*

Shader Loading

 */

GLuint LoadShaderFromFile(string pathString,GLenum shaderType){
  //Read all text
  fprintf(stderr, "Attempting shader load from %s...",pathString.c_str());
  const char *path=pathString.c_str();
  string varyingShaderCode;
  ifstream file (path,ios::in);
  if (file.is_open()){
    string line;
    while(getline(file,line))
      varyingShaderCode+=("\n"+line);
    file.close();
  }else{
    fprintf(stderr, " Failure\nFile couldn't be read\n");
    return 0;
  }
  const char *shaderCode=varyingShaderCode.c_str();
  //Compile text
  GLuint ShaderID=glCreateShader(shaderType);
  glShaderSource(ShaderID, 1, &shaderCode,NULL);
  glCompileShader(ShaderID);
  //Make sure compile was successful
  GLint compileResult;
  int compileInfoLength;
  glGetShaderiv(ShaderID,GL_COMPILE_STATUS,&compileResult);
  glGetShaderiv(ShaderID,GL_INFO_LOG_LENGTH,&compileInfoLength);
  if (compileResult==GL_FALSE){
    //Shader has warnings or has failed
    char ErrorMessage[compileInfoLength+1];
    glGetShaderInfoLog(ShaderID,compileInfoLength,NULL,ErrorMessage);
    fprintf(stderr," Failure\n%s\n",ErrorMessage);
    return 0;
  }
  fprintf(stderr," Success\n");
  //Send Shader ID
  return ShaderID;
}

GLuint LoadShadersIntoProgram(string vertexPath,string fragPath,GLuint program)
{
  if(program==0)
    program=glCreateProgram();
  GLuint vertexShader=LoadShaderFromFile(vertexPath,GL_VERTEX_SHADER);
  if(vertexShader==0)
    return 0;
  glAttachShader(program,vertexShader);
  GLuint fragShader=LoadShaderFromFile(fragPath,GL_FRAGMENT_SHADER);
  if(fragShader==0)
    return 0;
  glAttachShader(program,fragShader);
  glLinkProgram(program);
  //Check program has linked properly
  GLint linkResult=GL_FALSE;
  int linkInfoLength;
  glGetProgramiv(program,GL_LINK_STATUS,&linkResult);
  glGetProgramiv(program,GL_INFO_LOG_LENGTH, &linkInfoLength);
  if (linkResult==GL_FALSE){
    //Program linkage has failed
    char ErrorMessage[linkInfoLength+1];
    glGetProgramInfoLog(program,linkInfoLength,NULL,ErrorMessage);
    fprintf(stderr,"%s\n",ErrorMessage);
    return 0;
  }
  glDetachShader(program,vertexShader);
  glDetachShader(program,fragShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);
  return program;
}

/*

Texture Loading

 */

GLuint LoadTextureFromFile(string imagePath,GLuint imageType){
    return LoadPNGFromFile(imagePath,imageType);
}

GLuint LoadPNGFromFile(string imagePath, GLuint imageType){
    fprintf(stderr,"Attempting image load from %s...",imagePath.c_str());
    std::vector<unsigned char> image; //pixel data
    unsigned width,height;
    unsigned error=lodepng::decode(image,width,height,imagePath.c_str());
    if (error){
        fprintf(stderr,"Failed to decode png\n");
        return 0;
    }
    GLuint TextureID;
    glGenTextures(1,&TextureID);
    glBindTexture(GL_TEXTURE_2D,TextureID);
    fprintf(stderr,"%s",SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,&image[0]);
    //Set filtering mode for when pixels smaller and bigger than screen
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    fprintf(stderr,"Success\n");
    return TextureID;
}

GLuint LoadBMPFromFile(string imagePath, GLuint imageType){
  fprintf(stderr,"Attempting image load from %s...",imagePath.c_str());
  SDL_Surface *surface;
  SDL_Surface *image_surface=IMG_Load(imagePath.c_str());
  //SDL_PixelFormat sdl_pixel_format;
  if (image_surface==NULL){
    //SDL couldn't load image
    fprintf(stderr, " Failure\n%s\n", SDL_GetError());
    return 0;
  }
  /*sdl_pixel_format.palette = NULL;
  sdl_pixel_format.format = SDL_PIXELFORMAT_RGB888;
  sdl_pixel_format.BitsPerPixel = 24;
  sdl_pixel_format.BytesPerPixel = 8;
  sdl_pixel_format.Rmask=0x0000ff;
  sdl_pixel_format.Gmask=0x00ff00;
  sdl_pixel_format.Bmask=0xff0000;*/
  surface = SDL_ConvertSurfaceFormat(image_surface,SDL_PIXELFORMAT_RGB888,0);//SDL_ConvertSurface(image_surface, &sdl_pixel_format, 0 );
  if (surface==NULL){
    //SDL couldn't convert image
    fprintf(stderr, " Failure to convert image:\n%s\n", SDL_GetError());
    return 0;
  }

  //Generate an OpenGL texture to return
  GLuint TextureID;
  if (0) {
      unsigned char *p = (unsigned char *)surface->pixels;
      int i, j;
      printf( "width * height = %d . %d \n",surface->w, surface->h);
      for (i=0; i<surface->w*surface->h*3; i+=16) {
          printf("%03x:",i);
          for (j=0; j<16; j++) {
              printf(" %02x",*p++);
          }
          printf("\n");
      }
  }
  glGenTextures(1,&TextureID);
  glBindTexture(GL_TEXTURE_2D,TextureID);
  fprintf(stderr,"%s",SDL_GetError());
  //glPixelStorei(GL_UNPACK_ALIGNMENT,4);	
  //glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,16/*surface->w*/,16/*surface->h*/,0,GL_RGB,GL_UNSIGNED_BYTE,surface->pixels);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,surface->w,surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);
  //Set filtering mode for when pixels smaller and bigger than screen
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  //glGenerateMipmap(GL_TEXTURE_2D);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  SDL_FreeSurface(surface);
  SDL_FreeSurface(image_surface);
  fprintf(stderr," Success\n");
  return TextureID;
}

/*

Model Loading

*/

int LoadModelFromFile(string modelPath, GLuint buffers[3], glm::mat3 *transform, glm::vec3 *translation)
{
  //Assume model is a .obj
  fprintf(stderr, "Attempting model load from %s...",modelPath.c_str());
  FILE* modelFile=fopen(modelPath.c_str(),"r");
  if(modelFile==NULL) {
      fprintf(stderr, " Failure\nCouldn't open model file '%s'\n", modelPath.c_str());
    return 0;
  }
  vector<glm::vec3> verts,normals;
  vector<glm::vec2> uvs;
  vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  int line=0;
  uint i;
  while (1){
    //Find first word of line
    char lineHeader[128];
    int res= fscanf(modelFile,"%s",lineHeader);
    if(res==EOF)
      break;
    if(strcmp(lineHeader,"vt")==0){
      //Process UV
      glm::vec2 uv;
      fscanf(modelFile,"%f %f\n",&uv.x,&uv.y);
      uv.y=1-uv.y;//(2+uv.y)/4;
      //uv.x=(2+uv.x)/4;
      uvs.push_back(uv);
    }else if(strcmp(lineHeader,"vn")==0){
      //Process normal
      glm::vec3 normal;
      fscanf(modelFile,"%f %f %f\n", &normal.x,&normal.y,&normal.z);
      normals.push_back(normal);
    }else if(strcmp(lineHeader,"v")==0){
      //Process vertex
      glm::vec3 vertex;
      fscanf(modelFile,"%f %f %f\n",&vertex.x,&vertex.y,&vertex.z);
      verts.push_back(vertex);
    }else if(strcmp(lineHeader,"f")==0){
      //Process face
      unsigned int vertexIndex[3],uvIndex[3],normalIndex[3];
      int matches=fscanf(modelFile,"%d/%d/%d %d/%d/%d %d/%d/%d\n",
			 &vertexIndex[0],&uvIndex[0],&normalIndex[0],
			 &vertexIndex[1],&uvIndex[1],&normalIndex[1],
			 &vertexIndex[2],&uvIndex[2],&normalIndex[2]);
      if (matches!=9){
	fprintf(stderr," Failure\nLine %d couldn't be read\n",line);
	return 0;
      }
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      
      uvIndices.push_back(uvIndex[0]);
      uvIndices.push_back(uvIndex[1]);
      uvIndices.push_back(uvIndex[2]);

      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    }
    line++;
    //Done processing, loop back
  }
  //Transform verticies
  for(i=0; i<verts.size();i++){
      if (transform) {
          verts[i] = (*transform) * verts[i];
      }
      if (translation) {
          verts[i] = verts[i] + (*translation);
      }
  }
  for(i=0; i<normals.size();i++){
      if (transform) {
          normals[i] = (*transform) * normals[i];
      }
  }
  //Organize data into OpenGL compatible format
  unsigned int vertexIndicesLength=vertexIndices.size();
  float organizedVerticies[vertexIndicesLength*3];
  float organizedNormals[vertexIndicesLength*3];
  float organizedUVs[vertexIndicesLength*3];
  //Organize verticies
  for(i=0; i<vertexIndicesLength;i++){
      float *vertex = glm::value_ptr(verts[vertexIndices[i]-1]);
    //-1 because .obj starts from 1
      organizedVerticies[3*i+0]=vertex[0];
      organizedVerticies[3*i+1]=vertex[1];
      organizedVerticies[3*i+2]=vertex[2];
  }
  //Organize normals
  for(i=0; i<vertexIndicesLength;i++){
      float *normal = glm::value_ptr(normals[normalIndices[i]-1]);
    //-1 because .obj starts from 1
      organizedNormals[3*i+0]=normal[0];
      organizedNormals[3*i+1]=normal[1];
      organizedNormals[3*i+2]=normal[2];
  }
  //Organize uvs
  for(i=0; i<vertexIndicesLength;i++){
      float *uv = glm::value_ptr(uvs[uvIndices[i]-1]);
      organizedUVs[2*i+0]=uv[0];
      organizedUVs[2*i+1]=uv[1];
  }
  //Data is organized, create array of buffer and finish
  //Vertex=0
  //UV=1
  //Normal=2
  glGenBuffers(3,buffers);
  //Fill vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
  glBufferData(GL_ARRAY_BUFFER,
	       sizeof(glm::vec3)*vertexIndicesLength,
	       &organizedVerticies[0],
	       GL_STATIC_DRAW);
  //Fill uv buffer
  glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
  glBufferData(GL_ARRAY_BUFFER,
	       sizeof(glm::vec2)*vertexIndicesLength,
	       &organizedUVs[0],
	       GL_STATIC_DRAW);
  //Fill normal buffer
  glBindBuffer(GL_ARRAY_BUFFER,buffers[2]);
  glBufferData(GL_ARRAY_BUFFER,
	       sizeof(glm::vec3)*vertexIndicesLength,
	       &organizedNormals[0],
	       GL_STATIC_DRAW);
  fprintf(stderr, " Success\n");
  return vertexIndicesLength;
}

int LoadModelFromFile(string modelPath, GLuint buffers[3], float scale){
    glm::mat3 transform;
    transform = glm::mat3(scale);
    return LoadModelFromFile(modelPath, buffers, &transform, NULL);
}

/*
    PLAYGROUND FILES
                      */

/*void SplitString(string& str, char delim,vector<string> *v) {
    auto i = 0;
    auto pos = str.find(delim);
    while (pos != string::npos) {
      v->push_back(str.substr(i, pos-i));
      i = ++pos;
      pos = str.find(delim, pos);

      if (pos == string::npos)
         v->push_back(str.substr(i, str.length()));
    }
}*/







Mesh* LoadMesh(const char* name, const char* projectFolder){
    char meshPath[256];
    //fprintf(stderr,  "%s/Meshes/%s.obj",projectFolder,name);
    sprintf(meshPath,"%s/Meshes/%s.obj",projectFolder,name);
    return new Mesh(meshPath);
}
