#include <iostream>
#include"algebra3/algebra3.h"
#ifndef RAY_H
#define RAY_H

class Ray
{
private:
    vec3 origin;
    vec3 direction;
    float t;
public:
    Ray(vec3 ori, vec3 dir);
    vec3 getOri();
    vec3 getDir();
    vec3 getPoint(float t);
};

#endif
