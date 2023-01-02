#include "Ray.h"

Ray::Ray(vec3 ori, vec3 dir)
{
    origin = ori;
    direction = dir;
}

vec3 Ray::getOri()
{
    return origin;
}

vec3 Ray::getDir()
{
    return direction;
}

vec3 Ray::getPoint(float t)
{
    return this->origin + (t * this->direction);
}
