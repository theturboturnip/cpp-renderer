#ifndef __INC_TRANSFORM_H__
#define __INC_TRANSFORM_H__

#include "includes.h"

class Transform{
public:
    glm::vec3 position,rotation,scale;
    glm::mat4 Evaluate(void);
    glm::mat4 GetRotationMatrix(void);
    Transform(glm::vec3 *pos,glm::vec3 *rot,glm::vec3 *_scale);
};

#endif
