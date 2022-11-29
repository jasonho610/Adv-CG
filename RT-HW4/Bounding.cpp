#include "Bounding.h"
#include "Triangle.h"
#include "Sphere.h"
#include <algorithm>
#include <vector>

Box::Box()
{
	// std::cout << "Created an axis-aligned box." << std::endl;
	this->isHit = false; 
}

Box::Box(float x1, float y1, float z1,
         float x2, float y2, float z2)
{
    this->LDF.set(x1, y1, z1);     // left down front point
    this->RUB.set(x2, y2, z2);     // right up back point
    this->isHit = false;
}

Box::Box(vec3 p1, vec3 p2)
{
    this->LDF = p1;     // left down front point
    this->RUB = p2;     // right up back point
    this->isHit = false;
}

bool Box::intersect(Ray ray)
{
      //           LUB___________________RUB
      //            /                  /|
      //           /                  / |
      //       LUF/__________________/RUF
      //          |                  |  |
      //          |  LDB             |  /RDB
      //          |                  | /
      //          |__________________|/
      //         LDF               RDF
    
    vec3 RUB = this->RUB;
    vec3 LDF = this->LDF;
    vec3 LUB(LDF.getX(), RUB.getY(), RUB.getZ());                
    vec3 RUF(RUB.getX(), RUB.getY(), LDF.getZ());                
    vec3 LUF(LDF.getX(), RUB.getY(), LDF.getZ());                 
    vec3 RDB(RUB.getX(), LDF.getY(), RUB.getZ());                 
    vec3 LDB(LDF.getX(), LDF.getY(), RUB.getZ());                 
    vec3 RDF(RUB.getX(), LDF.getY(), LDF.getZ());
    // vector<string> box_tri_idx = {"RUB", "LUB", "LUF", "RUF", "RDB", "LDB", "LDF", "RDF"};
        
    vec3 ori = ray.getOri();
    vec3 dir = ray.getDir();
	std::vector<Triangle> box_tri;
	
	Triangle t1(RUB, RUF, LUB); box_tri.push_back(t1);
	Triangle t2(LUF, RUF, LUB); box_tri.push_back(t2);
	Triangle t3(RUB, RUF, RDB); box_tri.push_back(t3);
	Triangle t4(RDF, RUF, RDB); box_tri.push_back(t4);
	Triangle t5(RUF, LUF, RDF);	box_tri.push_back(t5);
	Triangle t6(LDF, LUF, RDF); box_tri.push_back(t6);
	Triangle t7(LDF, LDB, LUF); box_tri.push_back(t7);
	Triangle t8(LUB, LDB, LUF); box_tri.push_back(t8);
	Triangle t9(LUB, LDB, RUB); box_tri.push_back(t9);
	Triangle t10(RDB, LDB, RUB); box_tri.push_back(t10);
	Triangle t11(RDF, RDB, LDF); box_tri.push_back(t11);
	Triangle t12(LDB, RDB, LDF); box_tri.push_back(t12);
	
	for(int i = 0; i < 12; i++)
	{
		float t0 = 0;
    	float t1 = 0;
		if (box_tri[i].intersect(ray, t0, t1))
		{
			this->isHit = true;
			return true;
		}
	}
	return false;
}

void Box::bounding(std::vector<Sphere> spheres, std::vector<Triangle> triangles)
{
	/* TODO: add sphere bounding
	for (int sp_idx = 0;  sp_idx < spheres.size() ; sp_idx++)
    { ... }
    */
    vec3 p1, p2, p3;
    std::vector<float> xs, ys, zs;
    // Triangle
    for (int tri_idx = 0;  tri_idx < triangles.size()-2 ; tri_idx++)  // last two triangles are stages.
    {
    	triangles[tri_idx].getPoints(p1, p2, p3, false);
    	
		xs.push_back(p1.getX());
		xs.push_back(p2.getX());
		xs.push_back(p3.getX());
		
		ys.push_back(p1.getY());
		ys.push_back(p2.getY());
		ys.push_back(p3.getY());
		
		zs.push_back(p1.getZ());
		zs.push_back(p2.getZ());
		zs.push_back(p3.getZ());
    }
    
	float xmax = *std::max_element(xs.begin(), xs.end());
	float xmin = *std::min_element(xs.begin(), xs.end());
	float ymax = *std::max_element(ys.begin(), ys.end());
	float ymin = *std::min_element(ys.begin(), ys.end());
	float zmax = *std::max_element(zs.begin(), zs.end());
	float zmin = *std::min_element(zs.begin(), zs.end());
    
    this->LDF.set(xmin, ymin, zmin);
    this->RUB.set(xmax, ymax, zmax);
}

void Box::getPoints()
{
    std::cout << "B ";
    this->LDF.printCoor();
    this->RUB.printCoor();
    std::cout << std::endl;
}
