#include <iostream>
#include"algebra3/algebra3.h"
#ifndef LIGHT_H
#define LIGHT_H

class Light
{
private:
    vec3 position;
public:
    Light();
    Light(vec3 ori);
    void setLight(vec3 ori);
    vec3 getPostion();
};

#endif
