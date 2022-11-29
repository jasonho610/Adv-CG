#include "Light.h"
Light::Light()
{
}

Light::Light(vec3 ori)
{
    position = ori;
}

void Light::setLight(vec3 ori)
{
    position = ori;
}

vec3 Light::getPostion()
{
    return position;
}
