#include "Image.h"

Image::Image()
{
	vec3 temp;
    UL = temp;
    UR = temp;
    LL = temp;
    LR = temp;
}

Image::Image(vec3 ul, vec3 ur, vec3 ll, vec3 lr)
{
    UL = ul;
    UR = ur;
    LL = ll;
    LR = lr;
}

void Image::setImage(vec3 ul, vec3 ur, vec3 ll, vec3 lr)
{
	this->UL = ul;
    this->UR = ur;
    this->LL = ll;
    this->LR = lr;
}

vec3 Image::getCenter()
{
	vec3 temp;
	float tempX = (this->UL.getX()+UR.getX())/2.;
	float tempY = (this->UL.getY()+LL.getY())/2.;
	float tempZ = this->UL.getZ();
	temp.set(tempX, tempY, tempZ);
	return temp;
}

float Image::getWidth()
{
	float w = this->UR.getX() - this->UL.getX();
	return w;
}

float Image::getHeight()
{
	float h = this->UL.getY() - this->LL.getY();
	return h;
}

vec3 Image::getUL()
{
    return UL;
}

vec3 Image::getUR()
{
    return UR;
}

vec3 Image::getLL()
{
    return LL;
}

vec3 Image::getLR()
{
    return LR;
}
