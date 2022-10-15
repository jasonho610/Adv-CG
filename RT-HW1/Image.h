#include <iostream>
#include"algebra3/algebra3.h"
#ifndef IMAGE_H
#define IMAGE_H

class Image
{
private:
    vec3 UL;
    vec3 UR;
    vec3 LL;
    vec3 LR;
public:
	Image();
    Image(vec3 ul, vec3 ur, vec3 ll, vec3 lr);
	void setImage(vec3 ul, vec3 ur, vec3 ll, vec3 lr);
	vec3 getCenter();
    vec3 getUL();
    vec3 getUR();
    vec3 getLL();
    vec3 getLR();
    float getWidth();
    float getHeight();
};

#endif
