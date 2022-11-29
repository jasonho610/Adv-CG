#include <iostream>
#include"algebra3/algebra3.h"
#include "Ray.h"
#include "Triangle.h"
#include "Sphere.h"
#include <vector>
#ifndef BOUNDING_H
#define BOUNDING_H

class Box
{
private:
    vec3 LDF;     // left bottom front point
    vec3 RUB;     // right top back point
    bool isHit;   
public:
	Box();
    Box(float x1, float y1, float z1,
        float x2, float y2, float z2);
    Box(vec3 p1, vec3 p2);
    
    int prim_idx_start;   // primitive index start
    int prim_idx_end;     // primitive index end

	void bounding(std::vector<Sphere> spheres, std::vector<Triangle> triangles);
    bool intersect(Ray ray);
    void getPoints();
};

#endif
