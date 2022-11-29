#include <iostream>
#include"algebra3/algebra3.h"
#include "Ray.h"
#include "Material.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle
{
private:
    vec3 v0;
    vec3 v1;
    vec3 v2;
    vec3 normal;
    float s1 = INFINITY;
    float s2 = INFINITY;
    bool isHit;
    Material material;
public:
    Triangle(float x1, float y1, float z1,
             float x2, float y2, float z2,
             float x3, float y3, float z3);

	Triangle(vec3 p1, vec3 p2, vec3 p3);

    Triangle(float x1, float y1, float z1,
             float x2, float y2, float z2,
             float x3, float y3, float z3, Material m);
             
    Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3, 
				   float x4, float y4, float z4, Material m);

    bool intersect(Ray ray, float &t0, float &t1);
    void setTraingle(vec3 p1, vec3 p2, vec3 p3);
    void getPoints(vec3 &p1, vec3 &p2, vec3 &p3, bool print);
    vec3 getNormal();
    Material getMaterial();
};

#endif
