#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <time.h>
#include <string.h>
#include <string>
#include <limits.h>
#include <algorithm>
#include "algebra3/algebra3.h"
#include "Material.h"
#include "Ray.h"
#include "Sphere.h"
#include "output.h"
#include "Triangle.h"
#include "Light.h"
#include "Bounding.h"

int TRACE_DEPTH;
int PARTITION;

using namespace std;

bool check_obj_intsct(Ray ray, vector<Sphere> spheres, vector<Triangle> triangles, vector<Box> boxes,
            bool nearest, float &cur_nearest_dist, int &nearest_obj)
{
	if (nearest)
	{
		float t0 = 0;
    	float t1 = 0;
    	// Sphere
		for (int sp_idx = 0;  sp_idx < spheres.size() ; sp_idx++)
	    {
	        // Compare whether distance is nearest or not
	        if((spheres[sp_idx].intersect(ray, t0, t1)))
	        {
	            if (t0 < cur_nearest_dist)
	            {
	                nearest_obj = sp_idx;
	                cur_nearest_dist = t0;
	            }
	        }
	    }
		// Object Bounding Box
		for (int box_idx = 0;  box_idx < boxes.size(); box_idx++)
		{
		    if (boxes[box_idx].intersect(ray))
			{
			    for (int tri_idx = boxes[box_idx].prim_idx_start;  tri_idx < boxes[box_idx].prim_idx_end; tri_idx++)
			    {
			        // Compare whether distance is nearest or not
			        if(triangles[tri_idx].intersect(ray, t0, t1))
			        {
			            if (t0 < cur_nearest_dist)
			            {
			                nearest_obj = spheres.size() + tri_idx;
			                cur_nearest_dist = t0;
			            }
			        }
			    }
			}
		}
	    // Triangle Stage
	    for (int tri_idx = triangles.size()-2;  tri_idx < triangles.size(); tri_idx++)
	    {
	        // Compare whether distance is nearest or not
	        if(triangles[tri_idx].intersect(ray, t0, t1))
	        {
	            if (t0 < cur_nearest_dist)
	            {
	                nearest_obj = spheres.size() + tri_idx;
	                cur_nearest_dist = t0;
	            }
	        }
	    }
	    return true;
	}
	else
	{
		// intsct customized for shadow ray
		// we just need to find whether it intersect any obj
		float t0 = 0;
    	float t1 = 0;
		for (int sp_idx = 0;  sp_idx < spheres.size() ; sp_idx++)
	    {
	        if((spheres[sp_idx].intersect(ray, t0, t1))) return true;
	    }
	    // Object Bounding Box
		for (int box_idx = 0;  box_idx < boxes.size(); box_idx++)
		{
		    if (boxes[box_idx].intersect(ray))
			{
			    for (int tri_idx = boxes[box_idx].prim_idx_start;  tri_idx < boxes[box_idx].prim_idx_end; tri_idx++)
			    {
			        // Compare whether distance is nearest or not
			        if(triangles[tri_idx].intersect(ray, t0, t1)) return true;
			    }
			}
		}
	    // Triangle Stage
	    for (int tri_idx = triangles.size()-2;  tri_idx < triangles.size(); tri_idx++)
	    {
	        // Compare whether distance is nearest or not
	        if(triangles[tri_idx].intersect(ray, t0, t1)) return true;
	    }
	    return false;
	}
}            

// placed here for recursive issue
Color tracing(Ray, vector<Sphere>, vector<Triangle>, vector<Box>, Light, vec3, int, float &, bool &);

// Reflection
Color reflection(Ray incoming_ray, vec3 normal, vec3 intsct_point, int depth,
                 vector<Sphere> spheres, vector<Triangle> triangles, vector<Box> boxes,
                 Light light, vec3 eye, float &distance, bool &isIntsct)
{
    /*
    reflect = incoming - 2(incoming * normal) / normal, where normal should be a unit vector.
    see ->https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
    */
    vec3 reflect_dir = incoming_ray.getDir() - 2 * (incoming_ray.getDir() * normal) * normal;
    Ray reflect_ray(intsct_point, reflect_dir.normalize());

    Color clr = tracing(reflect_ray, spheres, triangles, boxes, light, eye, depth, distance, isIntsct);
    return clr;
}

// Tracing Ray
Color tracing(Ray ray, vector<Sphere> spheres, vector<Triangle> triangles, vector<Box> boxes,
              Light light, vec3 eye, int depth, float &distance, bool &isIntsct)
{
    float cur_nearest_dist = 1e9;
    int nearest_obj = -1;
    Color prm_color(0,0,0);  // phone reflection model color
    Color oor_color(0,0,0);  // other object reflection color
    Color ova_color(0,0,0);  // overall color

	check_obj_intsct(ray, spheres,  triangles, boxes, true, cur_nearest_dist, nearest_obj);
    
    if(nearest_obj >= 0)
    {
        vec3 intsct_point = ray.getPoint(cur_nearest_dist);
        vec3 N;                                                     // Normal
        vec3 L = (light.getPostion() - intsct_point).normalize();   // Light - intersect
        vec3 V = (eye - intsct_point).normalize();                  // View - intersect
        vec3 H = (L + V) / 2;                                       // L + V
        float exp;
        float li = 1.0;                                             // Light intensity
        float ka, kd, ks;
        float reflect;
        Color mat_color(0,0,0);                                     // material color
		
        if(nearest_obj < spheres.size())
        {
            mat_color.R = spheres[nearest_obj].getMaterial().color.R;
            mat_color.G = spheres[nearest_obj].getMaterial().color.G;
            mat_color.B = spheres[nearest_obj].getMaterial().color.B;
            // Get Normal
            N = intsct_point - spheres[nearest_obj].getCenter();
            // reminder: as aforementioned should be unit vector
            N.normalize();
            // Get constant
            exp = spheres[nearest_obj].getMaterial().exp;
            ka = spheres[nearest_obj].getMaterial().Ka;
            kd = spheres[nearest_obj].getMaterial().Kd;
            ks = spheres[nearest_obj].getMaterial().Ks;
            reflect = spheres[nearest_obj].getMaterial().Reflect;
        }
        else
        {
            // The nearsest object is Triangle.
            nearest_obj -= spheres.size();
            mat_color.R = triangles[nearest_obj].getMaterial().color.R;
            mat_color.G = triangles[nearest_obj].getMaterial().color.G;
            mat_color.B = triangles[nearest_obj].getMaterial().color.B;
            // Get Normal
            N = triangles[nearest_obj].getNormal();
            // reminder: as aforementioned should be unit vector
            N = N.normalize();
            // Get constant
            exp = triangles[nearest_obj].getMaterial().exp;
            ka = triangles[nearest_obj].getMaterial().Ka;
            kd = triangles[nearest_obj].getMaterial().Kd;
            ks = triangles[nearest_obj].getMaterial().Ks;
            reflect = triangles[nearest_obj].getMaterial().Reflect;
        }
        
        /*
        color = ambient
        for every light
        	cast shadow ray
        	color += local shading
        if mirror
        	color += color_reflect + trace reflection 
        if transparent
        	color += color_refract + trace refraction 
        */
        
        // Phong Reflection model
        float Ia = 1; // not specified, so default as one
        float Id = ((N * L) > 0) ?  li * (N * L) : li * (-1 * N * L);
        float Is = ((N * H) > 0) ?  li * pow((N * H), exp) : li * pow((-1 * N * H), exp);
        
        Ray shadow(intsct_point, L);								// the ray of shadow
        bool cover = check_obj_intsct(shadow, spheres, triangles, boxes, false, cur_nearest_dist, nearest_obj);
        
		if(cover)  // covered in shadow
		{
			// NOTE: if the result still shows seld-shadowing, go change the EPSILON value
			//       in Triangle.cpp and Sphere.cpp
			prm_color.R = ka * Ia * mat_color.R;
	        prm_color.G = ka * Ia * mat_color.G;
	        prm_color.B = ka * Ia * mat_color.B;
		}
		else      // direct to light source
		{
			/*
	        no need to consider the material's color of object when conputing specular
	        see -> https://moodle3.ntnu.edu.tw/mod/forum/discuss.php?d=187162
	        */
	        prm_color.R = ka * Ia * mat_color.R + kd * Id * mat_color.R + ks * Is * 255;
	        prm_color.G = ka * Ia * mat_color.G + kd * Id * mat_color.G + ks * Is * 255;
	        prm_color.B = ka * Ia * mat_color.B + kd * Id * mat_color.B + ks * Is * 255;	
		}
        
        if(depth == TRACE_DEPTH)
        {
            distance = cur_nearest_dist;
            isIntsct = true;
        }
        if(depth > 0)
        {
            // Reflection Recursive Method
            Color reflection_color = reflection(ray, N, intsct_point, depth-1, spheres, triangles, boxes, light, eye, distance, isIntsct);
            // Accumulated Color
            oor_color.R = (1 - reflect) * prm_color.R + reflect * reflection_color.R;
            oor_color.G = (1 - reflect) * prm_color.G + reflect * reflection_color.G;
            oor_color.B = (1 - reflect) * prm_color.B + reflect * reflection_color.B;
        }
        ova_color.R = (oor_color.R > 255)? 255 :oor_color.R;
        ova_color.G = (oor_color.G > 255)? 255 :oor_color.G;
        ova_color.B = (oor_color.B > 255)? 255 :oor_color.B;
        return ova_color;
    }
    else
    {
        ova_color.R = 0;
        ova_color.G = 0;
        ova_color.B = 0;
        isIntsct = false;
        return ova_color;
    }
}

vector <vector< Scene > > render(int width, int height, vec3 view_image_ul_point, Light light, vec3 eye,
                                 vec3 pixel_x_inc, vec3 pixel_y_inc,
                                 vector<Sphere> spheres, vector<Triangle> triangles, vector<Box> boxes,
                                 vector<vector <Scene> >  &screen, float pc_ratio)
{
    /*
    For every pixel
        Construct a ray from the eye
        For every object in the scene
            Find intersection with ray
        	Keep if closest
        Shade the object
    */
    
    /* Bounding Box Acceleration
    For every pixel
        Construct a ray from the eye
        If the ray hit the bounding box
	        For every object in the scene
	            Find intersection with ray
	        	Keep if closest
        	Shade the object
    */
    
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
        	//cout << "Tracing for pixel[" << i << ", " << j << "]" << endl;
        	
            float distance = -1;
            bool isIntsct = false;
            int depth = TRACE_DEPTH;

            vec3 view_image_point = view_image_ul_point + i*pc_ratio*pixel_x_inc + j*pc_ratio*pixel_y_inc;
            vec3 view_ray_dir = view_image_point - eye;
            Ray ray(eye, view_ray_dir.normalize());

            Color color = tracing(ray, spheres, triangles, boxes, light, eye, depth, distance, isIntsct);

            screen[i][j].setColor(color.R, color.G, color.B);
            screen[i][j].distance = distance;
            screen[i][j].isIntsct = isIntsct;
        }
    }
    return screen;
}

vector<Box> segment_bounding(vector<Sphere> spheres, vector<Triangle> triangles)
{
	int partition = PARTITION;  // how many triangles to create a box
	vector<Box> boxes;    
    int object_n = triangles.size()-2;
    int iter;
    
    /* TODO: add sphere bounding and partition
    // Sphere
	for (int sp_idx = 0;  sp_idx < spheres.size() ; sp_idx++)
    { ... }*/
    
    if (object_n % partition) iter = object_n / partition + 1;
	else iter = object_n / partition;
    
    for (int i = 0; i < iter; i++)
    {
    	vector<Triangle> t;
    	vector<Sphere> s;
    	Box b;
    	int j = i+1;
    	
    	b.prim_idx_start = i*partition;
    	b.prim_idx_end = min(j*partition, object_n);
	    for (int tri_idx = b.prim_idx_start;  tri_idx < b.prim_idx_end ; tri_idx++) // Object only
	    {
	    	t.push_back(triangles[tri_idx]);
	    }
	    b.bounding(s, t);
	    boxes.push_back(b);
	}
	return boxes;
}

// Split string with space
vector<string> split_space(char str [], char * pattern)
{
    vector<string> result; //?
    char *p;
    p = strtok(str,pattern);
    while(p)
    {
        result.push_back(p);
        p=strtok(NULL,pattern);
    }
    return result;
}

int main(int argc, const char *argv[])
{
	TRACE_DEPTH = atoi(argv[1]); 
	//TRACE_DEPTH = 4; 
	PARTITION = atoi(argv[2]);
	//PARTITION = 200;
	string fig = argv[3];
	//string fig = "Input_Bunny";
	time_t start, end;
	const string infname = "Assignment 3 Input\\" + fig + ".txt";
    ifstream infile(infname.c_str());
    char * pattern = " ";
    string line;
    vector<string> info;
    vec3 eye;
    vec3 view_dir;
    vec3 up_dir;
    float fov;
    int width, height;
    int distance;
    vector<Sphere> spheres;
    vector<Triangle> triangles;
    Light light;
    Material material;
    vector<Box> boxes;

    // Read information
    while(getline(infile, line))
    {
        char *cstr = new char[line.length() + 1];
        strcpy(cstr, line.c_str());
        info = split_space(cstr, pattern);
        
        if (info.empty()) continue;
        else if (info[0] == "E")
        {
            float ex = atof(info[1].c_str());
            float ey = atof(info[2].c_str());
            float ez = atof(info[3].c_str());
            eye.set(ex, ey, ez);
        }
        else if(info[0] == "V")
        {
            float vx = atof(info[1].c_str());
            float vy = atof(info[2].c_str());
            float vz = atof(info[3].c_str());
            float ux = atof(info[4].c_str());
            float uy = atof(info[5].c_str());
            float uz = atof(info[6].c_str());
            view_dir.set(vx, vy, vz);
            up_dir.set(ux, uy, uz);
        }
        else if(info[0] == "F")
        {
            fov = atof(info[1].c_str());
        }
        else if(info[0] == "R")
        {
            width = atoi(info[1].c_str());
            height = atoi(info[2].c_str());
        }
        else if(info[0] == "S")
        {
            float ox = atof(info[1].c_str());
            float oy = atof(info[2].c_str());
            float oz = atof(info[3].c_str());
            float radius = atof(info[4].c_str());
            Sphere s(ox, oy, oz, radius, material);
            spheres.push_back(s);
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
            float x4 = atof(info[10].c_str());
            float y4 = atof(info[11].c_str());
            float z4 = atof(info[12].c_str());            
            Triangle t(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, material);
            triangles.push_back(t);
        }
        else if(info[0] == "M")
        {
            float r = atof(info[1].c_str());
            float g = atof(info[2].c_str());
            float b = atof(info[3].c_str());
            float Ka = atof(info[4].c_str());
            float Kd = atof(info[5].c_str());
            float Ks = atof(info[6].c_str());
            float exp = atof(info[7].c_str());
            float Reflect = atof(info[8].c_str());
            material.setMaterial(r, g, b, Ka, Kd, Ks, exp, Reflect);
        }
        else if(info[0] == "L")
        {
            float x1 = atof(info[1].c_str());
            float y1 = atof(info[2].c_str());
            float z1 = atof(info[3].c_str());
            light.setLight(vec3(x1, y1, z1));
        }
        else
        {
            cout << "There is an unkown argument." << endl;
        }
    }
    
    cout << "Input file : " << infname << endl;
    
	float half_width = float(width) / 2;
    float half_height = float(height) / 2;
    /* e.g.
          When fov = 45(angle), from image's center to it's left side will have an angle of 45/2 = 22.5.
          It forms a triangle between eye, image center, and the left side.
          So we can use trigonometric to get their relations.
    */
    distance = 1;   // we can specify any distance desired, since the image result has been determined by fov.
    float l = distance * tan(M_PI * 0.5 * fov / 180.);
    float pc_ratio = l / half_width;   // pixel to coordinate ratio
    
    vec3 lookat_point = eye + distance * view_dir;
    // define the image directions (U \leftarrow and V \uparrow)
    vec3 U = view_dir ^ up_dir;
    U = U.normalize();
    vec3 V = U ^ view_dir;
    V = V.normalize();

    vector<vector <Scene> > screen(width, vector <Scene>(height));
    
    vec3 view_image_ul_point = lookat_point + l * V + l * U;
    // define the pixel directions (x \rightarrow and y \downarrow)
    vec3 pixel_x_inc = -U;
    vec3 pixel_y_inc = -V;
    
    boxes = segment_bounding(spheres, triangles); // bounding segmented objects but no stage

	time(&start);
    render(width, height, view_image_ul_point, light, eye, pixel_x_inc, pixel_y_inc,
           spheres, triangles, boxes, screen, pc_ratio);
    time(&end);
	
	const string outfname = "Output\\" + fig + ".ppm";
	ColorImage image;
    int x, y;
    Pixel p = {0,0,0};

    image.init(width, height);
    for (y=0; y<height; y++)
    {
        for (x=0; x<width; x++)
        {
            p.R = screen[x][y].color.R;
            p.G = screen[x][y].color.G;
            p.B = screen[x][y].color.B;
            image.writePixel(x, y, p);
        }
    }
    image.outputPPM(outfname.c_str());
    
    cout << "Render duration: " << double(end - start) << " s \n";
    cout << "Output file : " << outfname << endl;
    
    return 0;
}

