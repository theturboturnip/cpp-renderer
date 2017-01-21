#include "transform.h"

Transform::Transform(glm::vec3 *pos,glm::vec3 *rot,glm::vec3 *_scale){
    position=(*pos);
    rotation=(*rot);
    scale=(*_scale);
}

glm::mat4 Transform::Evaluate(){
    //Scale,Rotate,Translate
    glm::mat4 toReturn=glm::scale(scale);
    //toReturn=rotationMat*toReturn;
    glm::mat4 translateMat=glm::translate(glm::mat4(),-position);
    //toReturn*=rotationMat;
    //scale,then rotate, then translate
    toReturn=translateMat*GetRotationMatrix()*toReturn;
    return toReturn;
}

glm::mat4 Transform::GetRotationMatrix(){
    glm::mat4 rotationMat=glm::rotate(glm::radians(rotation.y),glm::vec3(0,1,0));
    rotationMat=glm::rotate(rotationMat,glm::radians(rotation.x),glm::vec3(1,0,0));
    rotationMat=glm::rotate(rotationMat,glm::radians(rotation.z),glm::vec3(0,0,1));
    return rotationMat;
}
