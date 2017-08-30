/*----------------------------------------------------------
* Ray Tracer - Assignment 2
*
*  The Cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/
#include "Cylinder.h"
#include <math.h>


/**
* Cylinder's intersection method.  The input is a ray (pos, dir). 
*/
float Cylinder::intersect(glm::vec3 posn, glm::vec3 dir)
{
   float radius_sq = (dir.x * dir.x) + (dir.z * dir.z);
   float radius_sq_inter = 2 * (dir.x * (posn.x - center.x) + dir.z * (posn.z - center.z));
   float norm_posn = ((posn.x - center.x) * (posn.x - center.x)) + ((posn.z - center.z) * (posn.z - center.z)) - (radius * radius);
   float delta = ((radius_sq_inter * radius_sq_inter) - 4.0 * radius_sq * norm_posn);
   
	//No intersection
	if(fabs(delta) < 0.001) return -1.0; 
    if(delta < 0.0) return -1.0;

    float t1 = (-radius_sq_inter - sqrt(delta)) / (2 * radius_sq);
    float t2 = (-radius_sq_inter + sqrt(delta))  / (2 * radius_sq);
    
    if(fabs(t1) < 0.001 )
    {
        if (t2 > 0) return t2;
        else return -1.0; ////No intersection
    }
    
    //No intersection
    if(fabs(t2) < 0.001 ) return -1.0;
    
    float max = t2;
    float min = t1;

	if(t1 > t2){
		max = t1;
		min = t2;
	}
	
	float y1 = (posn.y + min * dir.y) - center.y;
	float y2 = (posn.y + max * dir.y) - center.y;
	
	if(! (y1 < 0 || y1 > height) && min != -1.0) return min;
	else if(! (y2 < 0 || y2 > height) && max != -1.0) return max;
	else return -1.0; // no interestion
	
	
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/

glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n =  p - center;
    n = glm::normalize(n);
    return n;
}

