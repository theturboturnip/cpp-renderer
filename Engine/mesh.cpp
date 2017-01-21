#include "mesh.h"

Mesh::Mesh(std::string modelPath){
    //GLuint VertexArrayID;
    glGenVertexArrays(1,&VertexArrayID);
    glBindVertexArray(VertexArrayID);
    vertexNum=LoadModelFromFile(modelPath, model, 1.0f); // Returns 0 on ERROR
}
void Mesh::Draw(){
    //VP is Projection  * View
    //Scale,rotate,translate for model matrix
    //Calculate MVP Matrix
    //glm::mat4 M= glm::mat4(1.0f);
    //M[3][3] = 1.0;
    //Apply MVP Matrix
    //glUniformMatrix4fv(shaderMatrixLocation,1,GL_FALSE,&M[0][0]);
    //Enable Arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    //Assign Verticies
    glBindBuffer(GL_ARRAY_BUFFER,model[0]);
    glVertexAttribPointer(0,
			3,//Vertex has 3 points: x,y,z
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0);
    //Assign UV
    glBindBuffer(GL_ARRAY_BUFFER,model[1]);
    glVertexAttribPointer(1,
			2,//UV has 2 points: x,y
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0);
    //Assign Normals
    glBindBuffer(GL_ARRAY_BUFFER,model[2]);
    glVertexAttribPointer(2,
			3,//Normal has 3 points: x,y,z
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0);
    //Actually Draw
    glDrawArrays(GL_TRIANGLES,0,vertexNum*3);
    //Disable Arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
