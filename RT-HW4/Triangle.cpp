#include "Triangle.h"
#define EPSILON 5e-6

Triangle::Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3)
{
    v0.set(x1, y1, z1);
    v1.set(x2, y2, z2);
    v2.set(x3, y3, z3);
    this->normal = (v1 - this->v0) ^ (v2 - this->v0); // cross product
    this->isHit = false;
}

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3)
{
    v0 = p1;
    v1 = p2;
    v2 = p3;
    this->normal = (v1 - this->v0) ^ (v2 - this->v0); // cross product
    this->isHit = false;
}

Triangle::Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3, Material m)
{
    v0.set(x1, y1, z1);
    v1.set(x2, y2, z2);
    v2.set(x3, y3, z3);
    this->normal = (v1 - this->v0) ^ (v2 - this->v0); // cross product
    this->isHit = false;
    this->material = m;
}

Triangle::Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3, 
				   float x4, float y4, float z4, Material m)
{
    v0.set(x1, y1, z1);
    v1.set(x2, y2, z2);
    v2.set(x3, y3, z3);
    normal.set(x4, y4, z4);
    this->isHit = false;
    this->material = m;
}

bool Triangle::intersect(Ray ray, float &t0, float &t1)
{
    // base parameter
    vec3 ori = ray.getOri();
    vec3 dir = ray.getDir();
    vec3 v0 = this->v0;
    vec3 v1 = this->v1 - v0; // vector
    vec3 v2 = this->v2 - v0;

    // compute t, u, v with Cramer's Rule
    vec3 T = ori  - v0;
    vec3 P = dir ^ v2;
    float det = P * v1;

    // scale u
    float u = P * T / det;
    if(u < 0 || u > 1) return false;

    // scale v
    vec3 Q = T ^ v1;
    float v = Q * dir / det;
    if(v < 0 || (u + v) > 1) return false;

    // ray direction
    float t  = Q * v2 / det;
    t0 = t;
    if(t0 < EPSILON) return false; // adding EPSILON to prevent self-shadowing
    return true;
}

void Triangle::setTraingle(vec3 p1, vec3 p2, vec3 p3)
{
    this->v0 = p1;
    this->v1 = p2;
    this->v2 = p3;
}

void Triangle::getPoints(vec3 &p1, vec3 &p2, vec3 &p3, bool print)
{
	p1 = this->v0;
	p2 = this->v1;
	p3 = this->v2;
	
	if(print)
	{
		std::cout << "T ";
	    this->v0.printCoor();
	    this->v1.printCoor();
	    this->v2.printCoor();
	    std::cout << "\n";
	}
}

vec3 Triangle::getNormal()
{
    return this->normal;
}

Material Triangle::getMaterial()
{
    return this->material;
}
