#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <string.h>
#include <limits.h>
#include "algebra3/algebra3.h"
#include "Ray.h"
#include "Sphere.h"
#include "output.h"
#include "Triangle.h"
#include "Image.h"
#include "Material.h"

using namespace std;
vector<string> split(char [], char *);
Color tracing(Ray, vector<Sphere>, vector<Triangle>, vec3, int, float &, bool &);

// Split string with space
vector<string> split(char str [], char * pattern)
{
    std::vector<std::string> result;
    char *p;
    p = strtok(str,pattern);
    while(p)
    {
        result.push_back(p);
        p=strtok(NULL,pattern);
    }
    return result;
}

// Tracing Ray
Color tracing(Ray ray, vector<Sphere> Spheres_vector, vector<Triangle> Triangles_vector, vec3 eye, int depth, float &distance, bool &isInterset)
{
    float t0 = 0;
    float t1 = 0;
    float curNearestDist = INT_MAX;
    int nearestObj = -1;
    Color curColor(0,0,0);

    // Sphere
    for (int sp_idx = 0;  sp_idx < Spheres_vector.size() ; sp_idx++)
    {
        // Compare whether distance is nearest or not
        if((Spheres_vector[sp_idx].intersect(ray, t0, t1)))
        {
            if (t0 < curNearestDist)
            {
                nearestObj = sp_idx;
                curNearestDist = t0;
            }
        }
    }
    // Triangle
    for (int tri_idx = 0;  tri_idx < Triangles_vector.size() ; tri_idx++)
    {
        // Compare whether distance is nearest or not
        if(Triangles_vector[tri_idx].intersect(ray, t0, t1))
        {
            if (t0 < curNearestDist)
            {
                nearestObj = Spheres_vector.size() + tri_idx;
                curNearestDist = t0;
            }
        }
    }
    // Keep if closest
    if(nearestObj >= 0)
    {
    	vec3 intersect_p = ray.getPoint(curNearestDist);
        curColor.R = 255 * (intersect_p.getX() + 1.) / 2;
        curColor.G = 255 * (intersect_p.getY() + 1.) / 2;
        curColor.B = 255 * (intersect_p.getZ() + 1.) / 2;
        return curColor;
    }
    else
    {
        curColor.R = 0;
        curColor.G = 0;
        curColor.B = 0;
        isInterset = false;
        return curColor;
    }
}

vector <vector< Scene > > render(int width, int height, vec3 viewPlaneTopLeftPoint, vec3 Eye, 
								 int MAX_SAMPLING, float SAMPLING_RANGE, vec3 xIncVector, vec3 yIncVector, 
								 vector<Sphere> Spheres_vector, vector<Triangle> Triangles_vector, 
								 vector<vector <Scene> >  &screen)
{
    // for every pixel
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            float interval;
            float sampling_x;
            float sampling_y;
            float sampling = 0;
            Color acc_clr(0,0,0);
            float distance = -1;
            bool isInterset = false;
            int depth = 1;
            for(int step = 0; step < MAX_SAMPLING; step++)
            {
                sampling_x = i;// + interval;
                sampling_y = j;// + interval;

                vec3 viewPlanePoint = viewPlaneTopLeftPoint + sampling_x*xIncVector + sampling_y*yIncVector;
                vec3 castRay = viewPlanePoint - Eye;
                Ray ray(Eye, castRay.normalize());

                Color clr = tracing(ray, Spheres_vector, Triangles_vector, Eye, depth, distance, isInterset);

                acc_clr.setColor(clr.R, clr.G, clr.B);
                sampling++;
            }
            acc_clr.setColor(acc_clr.R / sampling, acc_clr.G / sampling, acc_clr.B / sampling);
            float r = acc_clr.R;
            float g = acc_clr.G;
            float b = acc_clr.B;
            screen[i][j].setColor(r, g, b);
            screen[i][j].distance = distance;
            screen[i][j].isInterset = isInterset;
        }
    }
    return screen;
}


int main()
{
    ifstream file("hw1_input.txt");
    char * pattern = " ";
    string line;
    vector<string> info;
    vec3 Eye;
    Image image;
    
    int width, height;
    vector<Sphere> Spheres_vector;
    vector<Triangle> Triangles_vector;

    // Read information
    while(getline(file, line))
    {
        char *cstr = new char[line.length() + 1];
        strcpy(cstr, line.c_str());
        info = split(cstr, pattern);
        if( info[0] == "E")
        {
            float ex = atof(info[1].c_str());
            float ey = atof(info[2].c_str());
            float ez = atof(info[3].c_str());
            Eye.set(ex, ey, ez);
        }
        else if( info[0] == "O")
        {
            float ulx = atof(info[1].c_str());
            float uly = atof(info[2].c_str());
            float ulz = atof(info[3].c_str());
            float urx = atof(info[4].c_str());
            float ury = atof(info[5].c_str());
            float urz = atof(info[6].c_str());
            float llx = atof(info[7].c_str());
            float lly = atof(info[8].c_str());
            float llz = atof(info[9].c_str());
            float lrx = atof(info[10].c_str());
            float lry = atof(info[11].c_str());
            float lrz = atof(info[12].c_str());
            
            vec3 ul(ulx, uly, ulz);
            vec3 ur(urx, ury, urz);
            vec3 lr(lrx, lry, lrz);
            vec3 ll(llx, lly, llz);
            
            image.setImage(ul, ur, lr, ll);
        }
        else if(info[0] == "R")
        {
            width = atoi(info[1].c_str());
            height = atoi(info[2].c_str());
        }
        else if(info[0] == "S")
        {
            float oX = atof(info[1].c_str());
            float oY = atof(info[2].c_str());
            float oZ = atof(info[3].c_str());
            float radius = atof(info[4].c_str());
            Sphere s(oX, oY, oZ, radius);
            Spheres_vector.push_back(s);
        }
        else if(info[0] == "T")
        {
            float x1 = atof(info[1].c_str());
            float y1 = atof(info[2].c_str());
            float z1 = atof(info[3].c_str());
            float x2 = atof(info[4].c_str());
            float y2 = atof(info[5].c_str());
            float z2 = atof(info[6].c_str());
            float x3 = atof(info[7].c_str());
            float y3 = atof(info[8].c_str());
            float z3 = atof(info[9].c_str());
            Triangle t(x1, y1, z1, x2, y2, z2, x3, y3, z3);
            Triangles_vector.push_back(t);
        }
        else
        {
            cout << "not mapping";
        }
    }
    /**
    For every pixel
        Construct a ray from the eye
        For every object in the scene
            Find intersection with ray
        Keep if closest
    **/

	vec3 View_Direction = image.getCenter() - Eye;
	float distance = View_Direction.length();
	View_Direction = View_Direction.normalize();
    
    float halfWidth = float(width) / 2;
    float halfHeight = float(height) / 2;
    float dist_px = halfWidth / ((image.getWidth() / 2) / distance);  // ((image.getWidth / 2) / distance) = tan(Field angle)
    vec3 lookAtPoint_px = Eye + dist_px * View_Direction;
    vec3 up (0, 1, 0);
    vec3 U = View_Direction ^ up;
    U = U.normalize();
    //U.printCoor();
    vec3 V = U ^ View_Direction;
    V = V.normalize();
    //V.printCoor();
    // U  V  both unit vector

    vector<vector<Scene> > screen(width, vector<Scene>(height));
    vec3 viewPlaneTopLeftPoint_px = lookAtPoint_px + halfHeight * V + halfWidth * U;
    vec3 xIncVector = -(U * 2 * halfWidth) / width;
    //xIncVector.printCoor(); // ¡÷
    vec3 yIncVector = -(V * 2 * halfHeight) / height;
	//yIncVector.printCoor(); // ¡õ
    
    render(width, height, viewPlaneTopLeftPoint_px, Eye, 1, 1, xIncVector, yIncVector, Spheres_vector, Triangles_vector, screen);

    ColorImage image2D;
    int x, y;
    Pixel p= {0,0,0};

    image2D.init(width, height);
    for (y=0; y<height; y++)
    {
        for (x=0; x<width; x++)
        {
            p.R = screen[x][y].clr.R;
            p.G = screen[x][y].clr.G;
            p.B = screen[x][y].clr.B;
            image2D.writePixel(x, y, p);
        }
    }
    image2D.outputPPM("hw1-output.ppm");
    return 0;
}

