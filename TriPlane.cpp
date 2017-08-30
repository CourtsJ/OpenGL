/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The TriPlane class - Used to make 3-pointed Planes
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "TriPlane.h"
#include <math.h>


/**
* Checks if a point pt is inside the current polygon
* Implement a point inclusion test using 
* member variables a, b, c
*/
bool Triplane::isInside(glm::vec3 pt)
{

	glm::vec3 n = normal(pt);
	glm::vec3 ua = b-a, ub = c-b, uc = a-c;  //difference between border points
    glm::vec3 va = pt-a, vb = pt-b, vc = pt-c;  //difference between tested point and the border points
	
	
	glm::vec3 c_a = cross(ua, va);
    glm::vec3 c_b = cross(ub, vb);
    glm::vec3 c_c = cross(uc, vc);
    
    float n_a = glm::dot(c_a, n);
    float n_b = glm::dot(c_b, n); 
    float n_c = glm::dot(c_c, n);

	return (n_a > 0.0 && n_b > 0.0 && n_c > 0.0); //point is inbetween all points
	                                                           //else the point is outside plane 
	

}

/**
* Plane's intersection method.  The input is a ray (pos, dir). 
*/
float Triplane::intersect(glm::vec3 posn, glm::vec3 dir)
{
	glm::vec3 n = normal(posn);
	glm::vec3 vdif = a - posn;
	float vdotn = glm::dot(dir, n);
	if(fabs(vdotn) < 1.e-4) return -1;
    float t = glm::dot(vdif, n)/vdotn;
	if(fabs(t) < 0.0001) return -1;
	glm::vec3 q = posn + dir*t;
	if(isInside(q)) return t;
    else return -1;
}

/**
* Returns the unit normal vector at a given point.
* Compute the plane's normal vector using 
* member variables a, b, c.
* The parameter pt is a dummy variable and is not used.
*/
glm::vec3 Triplane::normal(glm::vec3 pt)
{
    glm::vec3 n = normalize(cross((b-a), (c-a)));
    return n;
}



