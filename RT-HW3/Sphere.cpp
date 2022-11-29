#include "Sphere.h"
#define EPSILON 5e-6

Sphere::Sphere(float oX, float oY, float oZ, float radius)
{
    center.set(oX, oY, oZ);
    this->radius = radius;
    this->isHit = false;
}

Sphere::Sphere(float oX, float oY, float oZ, float radius, Material m)
{
    center.set(oX, oY, oZ);
    this->radius = radius;
    this->isHit = false;
    this->material = m;
}

void Sphere::setCenter(float oX, float oY, float oZ)
{
    this->center.set(oX, oY, oZ);
}

void Sphere::setCenter(vec3 center)
{
    this->center = center;
}

vec3 Sphere::getCenter()
{
    return this->center;
}

void Sphere::setRadius(float r)
{
    this->radius = r;
}

vec3 Sphere::getRadius()
{
    return this->radius;
}


bool Sphere::intersect(Ray ray, float &t0, float &t1)
{
    vec3 ori = ray.getOri();
    vec3 dir = ray.getDir();
    float t = (this->center * dir - ori * dir ) / (dir * dir);
    vec3 nearest_point = ray.getPoint(t);
    float distance = (nearest_point - this->center).length();
    if(distance <= this->radius)
    {
        if(distance == this->radius)
        {
            t0 = (nearest_point - ori).length();
            if(t0 <EPSILON) return false; // adding EPSILON to prevent self-shadowing 
        }
        else
        {
            float inn_dist = sqrt(pow(this->radius, 2) - pow(distance, 2));
            vec3 fst_p = nearest_point - dir * inn_dist;
            vec3 snd_p = nearest_point + dir * inn_dist;
            t0 = (fst_p - ori).length();
            t1 = (snd_p - ori).length();
            if(t0 > t1)
            {
                t0 = t1;
            }

            int judge = t0 * 100;
            float _t0 = judge / 100.0;
            if(_t0 > 0 - EPSILON && _t0 < 0 + EPSILON) return false; // adding EPSILON to prevent nemerical error 
        }
        return true;
    }
    else
    {
        return false;
    }
}

Material Sphere::getMaterial()
{
    return this->material;
}

