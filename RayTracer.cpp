#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/glut.h>
#include "Plane.h"
#include "TriPlane.h"
#include "Cylinder.h"
#include "TextureBMP.h"
using namespace std;

//Global Variables
const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

glm::vec3 sphere_colour(0.86,0.8,0.87);

//Textures
TextureBMP floor_texture;
TextureBMP cat;

//A global list containing pointers to objects in the scene
vector<SceneObject*> sceneObjects;  


//---The most important function in a ray tracer! ---------------------------------
//	Computes the colour value obtained by tracing a ray and finding its 
//   closest point of intersection with objects in the scene.
//----------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0.43); // Grey Colour
	glm::vec3 light(10, 40, -3);
	glm::vec3 center(-8.0, 10.0, -60.0); //For Sphere2 to normalize
	glm::vec3 specColor(1);
	glm::vec3 term(0);
	glm::vec3 floor(0); 
	glm::vec3 colorSum(0);
	float ambientTerm = 0.2;
	float phong = 10;
	

    ray.closestPt(sceneObjects); //Compute the closest point of intersetion of objects with the ray

    if(ray.xindex == -1) return backgroundCol;      //If there is no intersection return background colour
    
    glm::vec3 col = sceneObjects[ray.xindex]->getColor(); //else return bject's colour 
     
    // Texture Sphere 
    if(ray.xindex == 1) 
	{
		float square_root = sqrt(((ray.xpt.x - center.x) * (ray.xpt.x - center.x)) + ((ray.xpt.z - center.z) * (ray.xpt.z - center.z)));
		float phi = atan2((ray.xpt.x - center.x),(ray.xpt.z - center.z));
		float delta = atan2((ray.xpt.y - center.y), square_root);
		float texcoords = (phi+M_PI)/(2 * M_PI);
		float texcoordt  = (delta + (M_PI/2))/M_PI;
		
		col = cat.getColorAt(texcoords, texcoordt);  
	}
	
	// Texture Plane
	if(ray.xindex == 16) 
	{
		float texcoords = (ray.xpt.x + 200)/(200+200);   
		float texcoordt = (ray.xpt.z + 40)/(-200+40);    
		col = floor_texture.getColorAt(texcoords, texcoordt);
	}  
			
	//Procedural Pattern
	
	
	
	if(ray.xindex == 4 && step < MAX_STEPS)
	{
		if ((int)ray.xpt.y%2)
		{
			col = sphere_colour;
		}		
	}
	
    glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);
    glm::vec3 lightVector = light - ray.xpt;
	glm::vec3 normLight = glm::normalize(lightVector);
	glm::vec3 reflVector = glm::reflect(-normLight, normalVector);

	Ray shadow(ray.xpt, normLight);
	shadow.closestPt(sceneObjects);
	
	float lightDist = glm::length(lightVector);
	float lDotn = glm::dot(normLight, normalVector); //Light Diffuse
	float rDotv = glm::dot(reflVector, -ray.dir); //Reflection Vector
	if (lDotn <= 0 or (shadow.xindex > -1 and shadow.xdist < lightDist))
	{
		colorSum = col*ambientTerm;
	}
	else
	{
		if (rDotv < 0)
		{
			term = colorSum * backgroundCol;
		}
		else
		{
			term = specColor * powf(rDotv,phong);
		}
	colorSum = col*ambientTerm + col*lDotn + term;
	}
	
	//Reflection 
	if(ray.xindex == 0 && step < MAX_STEPS)
	{
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
		Ray reflectedRay(ray.xpt, reflectedDir);
		glm::vec3 reflectedCol = trace(reflectedRay, step+1); //Recursion!
		colorSum = colorSum + (0.8f*reflectedCol);
	}
	
	//Transparent
	if(ray.xindex == 2 && step < MAX_STEPS)
	{
		float transparent = 0.3;
		
		float eta = 1/1.33; //Water 
		glm::vec3 refractedDir = glm::refract(ray.dir, normalVector, eta);
		Ray refractedRay(ray.xpt, refractedDir);
		refractedRay.closestPt(sceneObjects);
		glm::vec3 normalRefracted = sceneObjects[refractedRay.xindex]->normal(refractedRay.xpt);
		glm::vec3 refractedOutDir = glm::refract(refractedDir, -normalRefracted, transparent);
		Ray refractedRay2(refractedRay.xpt,refractedOutDir);
		glm::vec3 refractedCol = trace(refractedRay2, step+1);
		
		colorSum = colorSum * transparent + (1-transparent) * refractedCol ;
		

	}
	
	//Refraction
	if(ray.xindex == 3 && step < MAX_STEPS)
	{
		float eta = 1/1.33; //Water 
		glm::vec3 refractedDir = glm::refract(ray.dir, normalVector, eta);
		Ray refractedRay(ray.xpt, refractedDir);
		refractedRay.closestPt(sceneObjects);
		glm::vec3 normalRefracted = sceneObjects[refractedRay.xindex]->normal(refractedRay.xpt);
		glm::vec3 refractedOutDir = glm::refract(refractedDir, -normalRefracted, 1/eta);
		Ray refractedRay2(refractedRay.xpt,refractedOutDir);
		glm::vec3 refractedCol = trace(refractedRay2, step+1); 
		colorSum = colorSum + refractedCol;
	}
	
	return colorSum;
}


//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height

	glm::vec3 eye(0., 0., 0.);  //The eye position (source of primary rays) is the origin

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a quad.

	for(int i = 0; i < NUMDIV; i++)  	//For each grid point xp, yp
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

		    glm::vec3 dir1(xp+0.25*cellX, yp+0.25*cellY, -EDIST);	//Bottom Left
		    glm::vec3 dir2(xp+0.25*cellX, yp+0.75*cellY, -EDIST);	//Top Left
		    glm::vec3 dir3(xp+0.75*cellX, yp+0.25*cellY, -EDIST);	//Bottom Right
		    glm::vec3 dir4(xp+0.75*cellX, yp+0.75*cellY, -EDIST);	//Top Right 
		    
		    Ray ray1 = Ray(eye, dir1);		//Create a ray originating from the camera in the direction 'dir'
			ray1.normalize();				//Normalize the direction of the ray to a unit vector
		    glm::vec3 col = trace (ray1, 1); //Trace the primary ray and get the colour value
		    
		    Ray ray2 = Ray(eye, dir2);		
			ray2.normalize();				
		    col += trace (ray2, 1); 

			Ray ray3 = Ray(eye, dir3);		
			ray3.normalize();				
		    col += trace (ray3, 1); 

			Ray ray4 = Ray(eye, dir4);		
			ray4.normalize();				
		    col += trace (ray4, 1); 


			glColor3f(col.r/4, col.g/4, col.b/4);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}


//Creates tetrahedron - Uses Index 6 to 9
void tetrahedron()
{
	
	float x = 7;
	float x2 = 9;
	float x3 = 11;
	
	float y = -1;
	float y2 = -5;
	
	float z = -45;
	float z2 = -50;
	float z3 = -55;
	
	Triplane *tetra_down = new Triplane(glm::vec3(x3, y2, z3), //Point A
										glm::vec3(x, y2, z3),  //Point B
										glm::vec3(x2, y2, z), //Point C
										glm::vec3(0.86,0.8,0.87)); //Colour
	
	Triplane *tetra_left = new Triplane(glm::vec3(x2, y2, z), //Point A
										glm::vec3(x2, y,z2), //Point B - Top Tip
										glm::vec3(x, y2, z3), //Point C
										glm::vec3(0.86,0.8,0.87)); //Colour
										
	Triplane *tetra_right = new Triplane(glm::vec3(x, y2, z3), //Point A
										glm::vec3(x2, y, z2), //Point B - Top Tip
										glm::vec3(x3, y2, z3), //Point C
										glm::vec3(0.86,0.8,0.87)); //Colour
										
	Triplane *tetra_back = new Triplane(glm::vec3(x3, y2, z3), //Point A
										glm::vec3(x2,y,z2), //Point B - Top Tip
										glm::vec3(x2, y2, z), //Point C
										glm::vec3(0.86,0.8,0.87)); //Colour

	sceneObjects.push_back(tetra_down);
	sceneObjects.push_back(tetra_left);
	sceneObjects.push_back(tetra_right);
	sceneObjects.push_back(tetra_back);
}

// Creates Cube - Uses Index 10-15
void cube()
{
	float scale = 1.75;
	float x = 4 * scale;
	float x2 = 2 * scale;
	float x3 = -1 * scale;
	float y = 5;
	float z = -60;
	
	Plane *cube_front = new Plane (glm::vec3(x-x2, -y, z), //Point A
								glm::vec3(x, -y, z), //Point B
								glm::vec3(x, y-y, z), //Point C	
								glm::vec3(x-x2, y-y, z), //Point D
								glm::vec3(0.72, 0.81, 1.)); //Colour
								
	Plane *cube_back = new Plane (glm::vec3(-x3, -y, z), //Point A
								glm::vec3(-x3, y-y, z), //Point B
								glm::vec3(x-x2, y-y, z), //Point C	
								glm::vec3(x-x2, -y, z), //Point D
								glm::vec3(0.72, 0.81, 1.)); //Colour
								
	Plane *cube_left = new Plane(glm::vec3(-x3, -y, z), //Point A
								glm::vec3(x-x2, -y, z), //Point B
								glm::vec3(x-x2, y-y, z), //Point C	
								glm::vec3(-x3, y-y, z), //Point D
								glm::vec3(0.72, 0.81, 1.)); //Colour
	Plane *cube_right = new Plane(glm::vec3(x, -y, z), //Point A
								glm::vec3(x-x2, -y, z), //Point B
								glm::vec3(x-x2, y-y, z), //Point C	
								glm::vec3(x, y-y, z), //Point D
								glm::vec3(0.72, 0.81, 1.)); //Colour
	Plane *cube_up= new Plane(glm::vec3(x-x2, y-y, z), //Point A
								glm::vec3(x, y-y, z), //Point B
								glm::vec3(x-x2, y-y, z), //Point C	
								glm::vec3(-x3, -y, z), //Point D
								glm::vec3(0.72, 0.81, 1.)); //Colour
	Plane *cube_down = new Plane(glm::vec3(x-x2, -y, z), //Point A
								glm::vec3(-x3, -y, z), //Point B
								glm::vec3(x-x2, -y, z), //Point C	
								glm::vec3(x, -y, z), //Point D
								glm::vec3(0.72, 0.81, 1.)); //Colour

	sceneObjects.push_back(cube_front); 
	sceneObjects.push_back(cube_back);
	sceneObjects.push_back(cube_left);
	sceneObjects.push_back(cube_right);
	sceneObjects.push_back(cube_up);
	sceneObjects.push_back(cube_down);

}
//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);
	
	//Loads Textures
	floor_texture = TextureBMP("tile.bmp");
	cat = TextureBMP("cat1.bmp");
	
	//Creates a pointer to a sphere object - Index 16
	Plane *plane = new Plane (glm::vec3(-200., -10, -40), //Point A  
								glm::vec3(200., -10, -40), //Point B
								glm::vec3(200., -10, -200), //Point C	
								glm::vec3(-200., -10, -200), //Point D
								glm::vec3(0, 1.0, 0.788)); //Colour
								
	//-- Creates a pointer to a sphere object
	Sphere *sphere1 = new Sphere(glm::vec3(0.0, 7.0, -90.0), 12, glm::vec3(0.09, 0.08, 0.21));	// Index 0
	Sphere *sphere2 = new Sphere(glm::vec3(-8.0, 10.0, -60.0), 3, glm::vec3(0, 1, 0));	// Index 1 - Textured
	Sphere *sphere3 = new Sphere(glm::vec3(-9.0, 2.0, -55.0), 4, glm::vec3(0.58, 0.43, 0.56));	// Index 2 - Transparent
	Sphere *sphere4 = new Sphere(glm::vec3(0.5, -5.0, -41.0), 3, glm::vec3(0.47, 0., 0.67)); // Index 3 - Refracted
	Sphere *sphere5 = new Sphere(glm::vec3(7, 11.0, -70), 3, glm::vec3(1, 0.94, 0.80));		// Index 4 - Procedural Pattern
	
	//-- Creates a pointer to a Cylinder object
	Cylinder *cylinder1 = new Cylinder(glm::vec3(-9.0, -10.0, -55.0), 3, 8, glm::vec3(0.94, 0.69, 0.55)); // Index 5
	
	//--Adds the Spheres and the Cylinder to the list of scene objects
	sceneObjects.push_back(sphere1); 
	sceneObjects.push_back(sphere2); 
	sceneObjects.push_back(sphere3); 
	sceneObjects.push_back(sphere4); 
	sceneObjects.push_back(sphere5); 
	sceneObjects.push_back(cylinder1); 
	
	//Calls and adds the Tetrahedron and Cube objects
	tetrahedron();
	cube();

	//--Add the above to the list of scene objects.
	sceneObjects.push_back(plane);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");
    glutDisplayFunc(display);
    initialize();
    glutMainLoop();
    return 0;
}

