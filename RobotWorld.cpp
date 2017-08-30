//  ===================================================================
//  COSC363: Assignment1
//	Courteney Brownie
//  FILE NAME: RobotWorld
//  ====================================================================
 
#include <iostream>
#include <fstream>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadTGA.h"

using namespace std;

//--Globals ------------------------------------------------------------
GLuint txId[9];
GLUquadric *q; 


float cam_angle = 0.1;
float xz_angle = 0;				//Rotation xz_angle
float yz_angle = -.1;				//Rotation yz_angle
float cam_x = 0, cam_hgt = 8,  cam_z = 13;		//Initial Camera Position
float lookpoint_x = 50., lookpoint_y = 50., lookpoint_z = 0.5;
float dir_x = -sin(xz_angle), dir_y = 0,  dir_z = -cos(xz_angle); 
int pan_x = 0, pan_y = 0, pan_z = 0;				//Moving the Camera in x, y or z direction

int tree_x;
int tree_z= -43;

float robot_mark = 1.;

float left_antenna = 10;
float right_antenna = -40;
float left_bulb = 8.1;
float right_bulb = 8.65;

float eyes_red = 1.;
float eyes_green = 0.1;
float eyes_blue = 0.1;

float rbulb_red = 0.584;
float rbulb_green = 0.588;
float rbulb_blue = 0.572;

float lbulb_red = 0.584;
float lbulb_green = 0.588;
float lbulb_blue = 0.572;

float colour_change = 1;

float robot_angle = .001;
float robot_x = 5.8;
float robot_y = -2.6;
float robot_z = -41;

float flower_angle = 0.001;
float flower_x = -30;
float flower_y = -0.1;
float flower_z = 30; 

int tree1Click_x = 1;
int tree1Click_y = 1;
int tree1Click_z = 1;

int tree2Click_x = 1;
int tree2Click_y = 1;
int tree2Click_z = 1;

int tree3Click_x = 1;
int tree3Click_y = 1;
int tree3Click_z = 1;

int tree4Click_x = 1;
int tree4Click_y = 1;
int tree4Click_z = 1;

int tree5Click_x = 1;
int tree5Click_y = 1;
int tree5Click_z = 1;

float tree_down = 1;
float dance_flag = 1;
float flower_dance = 1;


//--Loads Texture Files-------------------------------------------------
void loadTexture()	 
{
	glGenTextures(9, txId);
	
	//Skybox - Back
	glBindTexture(GL_TEXTURE_2D, txId[0]); 
	loadTGA("plains-of-abraham_bk.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	//Skybox - Down
	glBindTexture(GL_TEXTURE_2D, txId[1]); 
	loadTGA("plains-of-abraham_dn.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//Skybox - Front
	glBindTexture(GL_TEXTURE_2D, txId[2]); 
	loadTGA("plains-of-abraham_ft.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//Skybox - Left
	glBindTexture(GL_TEXTURE_2D, txId[3]); 
	loadTGA("plains-of-abraham_lf.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	//Skybox - Right
	glBindTexture(GL_TEXTURE_2D, txId[4]); 
	loadTGA("plains-of-abraham_rt.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	//Skybox - Up
	glBindTexture(GL_TEXTURE_2D, txId[5]); 
	loadTGA("plains-of-abraham_up.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	//Tree Base - Bark
	glBindTexture(GL_TEXTURE_2D, txId[6]);  
	loadTGA("Bark.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Flower Petals
	glBindTexture(GL_TEXTURE_2D, txId[7]);  
	loadTGA("Petal.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Pot Plant Texture
	glBindTexture(GL_TEXTURE_2D, txId[8]);  
	loadTGA("pot.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//--Creates Skybox------------------------------------------------------
void skybox()
{
	int size = 50;
	//Back
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
		glBegin(GL_QUADS);					
		glTexCoord2f(0,1); glVertex3f(-size, size, -size);
		glTexCoord2f(0,0); glVertex3f(-size, -0.1, -size);
		glTexCoord2f(1,0); glVertex3f(size, -0.1, -size);
		glTexCoord2f(1,1); glVertex3f(size, size, -size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//Down
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glBegin(GL_QUADS);			 	
		glTexCoord2f(0,1); glVertex3f(-size, -0.1, -size);
		glTexCoord2f(0,0); glVertex3f(-size, -0.1, size);
		glTexCoord2f(1,0); glVertex3f(size, -0.1, size);
		glTexCoord2f(1,1); glVertex3f(size, -0.1, -size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//Front
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[2]);
	glBegin(GL_QUADS);					
		glTexCoord2f(0,1); glVertex3f(-size, size, size);
		glTexCoord2f(0,0); glVertex3f(-size, -0.1, size);
		glTexCoord2f(1,0); glVertex3f(size, -0.1, size);
		glTexCoord2f(1,1); glVertex3f(size, size, size);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Left 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[3]);
	glBegin(GL_QUADS);			 		
		glTexCoord2f(0,1); glVertex3f(-size, size, -size);
		glTexCoord2f(0,0); glVertex3f(-size, -0.1, -size);
		glTexCoord2f(1,0); glVertex3f(-size, -0.1, size);
		glTexCoord2f(1,1); glVertex3f(-size, size, size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	//Right
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[4]);
		glBegin(GL_QUADS);			 		
		glTexCoord2f(0,1); glVertex3f(size, size, -size);
		glTexCoord2f(0,0); glVertex3f(size, -0.1, -size);
		glTexCoord2f(1,0); glVertex3f(size, -0.1, size);
		glTexCoord2f(1,1); glVertex3f(size, size, size);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Top
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[5]);
	glBegin(GL_QUADS);			 		
		glTexCoord2f(0,1); glVertex3f(-size, size-0.1, -size);
		glTexCoord2f(0,0); glVertex3f(-size, size-0.1, size);
		glTexCoord2f(1,0); glVertex3f(size, size-0.1, size);
		glTexCoord2f(1,1); glVertex3f(size, size-0.1, -size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


//--Draws Robot Minus Antennas------------------------------------------
void drawRobotSilhouette()
{
	//Head
	glColor3f(0.82, 0.81, 0.81);		
	glPushMatrix();
		glTranslatef(0, 7.9, 0);
		glScalef(1.05, 1, 1);
		glutSolidCube(1.9);
	glPopMatrix();
	
	//Left Eye
	glColor3f(eyes_red, eyes_green, eyes_blue);		
	glPushMatrix();
	  glTranslatef(-0.45, 7.89, 0.9);
	  glutSolidSphere(.3, 36, 18);
	glPopMatrix();

	//Right Eye
	glColor3f(eyes_red, eyes_green, eyes_blue);		
	glPushMatrix();
	  glTranslatef(0.44, 7.89, 0.9);
	  glutSolidSphere(.46, 36, 18);
	glPopMatrix();
	
	//Neck
	glColor3f(0.584, 0.588, 0.572);			
	glPushMatrix();
	  glTranslatef(0, 6.7, 0);
	  glScalef(.8, 1, 1);
	  glutSolidCube(1);
	glPopMatrix();
	
	//Body
	glColor3f(0.82, 0.81, 0.81);			
	glPushMatrix();
	  glTranslatef(0, 5.5, 0);
	  glScalef(2.98, 2.5, 1.4);
	  glutSolidCube(1);
	glPopMatrix();

	//Door
	glColor3f(.290, 0.294, 0.286);			
	glPushMatrix();
		glTranslatef(0, 5.5, 0.8);
		glScalef(1.49, 1.4, 0.08);
		glutSolidCube(1);
	glPopMatrix();

	//Leg Connector
	glColor3f(0.584, 0.588, 0.572);
	glPushMatrix();
		glTranslatef(0., 4.19, 0);
		glScalef(1.5, 0.14, 0.5);
		glutSolidCube(1);
	glPopMatrix();
	
	//Leg 
	glColor3f(0.82, 0.81, 0.81);			
	glPushMatrix();
		glTranslatef(0., 3.83, 0);
		glScalef(3.18, 0.6, 1);
		glutSolidCube(1);
	glPopMatrix();
}
	
//--Draws Robot's Antennas----------------------------------------------
void drawRobotAntennas()
{
	//Left Antenna
	glColor3f(0.584, 0.588, 0.572);			
	glPushMatrix();
	glTranslatef(1.1, 7.89, 0);
	  glRotatef(-90, 0, 1., 0);
	  glRotatef(left_antenna, 1., 0., 0);
	  glScalef(0.2, 0.2, 2.35);
	  glutSolidCube(1);
	glPopMatrix();

	//Left Bulb
	glColor3f(lbulb_red, lbulb_green, lbulb_blue);		
	glPushMatrix();
	  glTranslatef(2.1, left_bulb, 0);
	  glutSolidSphere(.28, 36, 18);
	glPopMatrix();
	
	
	//Right Antenna
	glColor3f(0.584, 0.588, 0.572);			
	glPushMatrix();
		glTranslatef(-1.1, 7.89, 0);
		glRotatef(-90, 0, 1., 0);
		glRotatef(right_antenna, 1., 0., 0);
		glScalef(0.2, 0.2, 2.35);
		glutSolidCube(1);
	glPopMatrix();
	
	//Right Bulb
	glColor3f(rbulb_red, rbulb_green, rbulb_blue);		
	glPushMatrix();
	  glTranslatef(-1.98, right_bulb, 0);
	  glutSolidSphere(.28, 36, 18);
	glPopMatrix();
}


//--Draws Flower--------------------------------------------------------
void drawFlower()
{
	//Flower bulb
	glPushMatrix();
		glColor3f(0.94,0.84,0.14);
		glTranslatef(0, 7., 0);
		glutSolidSphere(.75, 36, 18);
    glPopMatrix();
        
	//Stem
	glPushMatrix();
    q = gluNewQuadric();
    glColor3f(0.16, 0.28, 0.05);
		glTranslatef(0.0, 4.4, 0.0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 0.3, 0.3, 2., 20, 5);
		gluDisk(q, 0.0, 0.3, 20, 1); 
    glPopMatrix();
	
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture (q, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, txId[7]);	
	
	//Petal 1
	glPushMatrix();
		glColor3f(0.,0.,1.);
		glTranslatef(0, 8.3, 0);
		gluSphere (q, .75, 8, 10 );
    glPopMatrix();

    //Petal 2
	 glPushMatrix();
		glColor3f(0.,0.,1.);
		glTranslatef(-1.1, 7.8, 0);
		gluSphere (q, .75, 8, 10 );
    glPopMatrix();

	//Petal 3
	 glPushMatrix();
		glColor3f(0.,0.,1.);
		glTranslatef(1.1, 7.8, 0);
		gluSphere (q, .75, 8, 10 );
    glPopMatrix();

    //Petal 3
	 glPushMatrix();
		glColor3f(0.,0.,1.);
		glTranslatef(-1.1, 6.4, 0);
		gluSphere (q, .75, 8, 10 );
    glPopMatrix();    
    
    //Petal 4
	 glPushMatrix();
		glColor3f(0.,0.,1.);
		glTranslatef(1.1, 6.4, 0);
		gluSphere (q, .75, 8, 10 );
    glPopMatrix();    
    
     //Petal 5
	 glPushMatrix();
		glColor3f(0.,0.,1.);
		glTranslatef(0, 5.8, .1);
		gluSphere (q, .75, 8, 10 );
    glPopMatrix();    
    
	glDisable(GL_TEXTURE_2D);    
}

//--Draws Flower's Stem longer--------------------------------------------------------
void drawFlowerStem()
{
	glPushMatrix();
    q = gluNewQuadric();
    glColor3f(0.16, 0.28, 0.05);
		glTranslatef(0.0, 2., 0.0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 0.3, 0.3, 2.5, 20, 5);
		gluDisk(q, 0.0, 0.3, 20, 1); 
    glPopMatrix();
}

//--Draws Stand---------------------------------------------------------
void drawStand()
{
	glPushMatrix();
      glRotatef(-90.0, 1., 0., 0.);
      gluCylinder(q, 3.8, 3.8, 2.7, 20, 5);
      glTranslatef(0.0, .0, 2.7);
      gluDisk(q, 0.0, 3.8, 20, 3);
    glPopMatrix();
}
//--Draws Potplant------------------------------------------------------
void drawPotPlant()
{
	//Pot
	q = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture (q, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, txId[8]);	
    
    glPushMatrix();
    glColor4f(0.5, 0., 0., 1.0);
		glTranslatef(0.0, 2.8, 0.0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 0.4, 0.6, 0.7, 20, 5);
		gluDisk(q, 0.0, 0.5, 20, 1); 
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    //Pot Base
    glPushMatrix();
		glColor3f(0., 0., 0.);
		glTranslatef(0.0, 2.7, 0.0);
		glRotatef(-90, 1, 0, 0);
		glutSolidTorus(.12, .47, 20, 32);
	glPopMatrix();
}

//--Draws Tree---------------------------------------------------------
void drawTree()
{
	//Base
	q= gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture (q, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, txId[6]);
   	glColor3f(1., 0., 0.);			
	glPushMatrix();
		glTranslatef(-5, 1, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 0.8, 0.8, 7.5, 20, 5);
		gluDisk(q, 0.0, 0.5, 20, 1); 
	glPopMatrix();
	glDisable(GL_TEXTURE_2D); 
	
	//Tree Layer1
	glColor3f(0.37, 0.59, 0.19);			
	glPushMatrix();
		glTranslatef(-5, 7, 0);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(2.8, 3.2, 20, 20);
	glPopMatrix();
	
	//Tree Layer 2
	glColor3f(0.24, 0.44, 0.07);			
	glPushMatrix();
		glTranslatef(-5, 9, 0);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(2.24, 2.56, 20, 20);
	glPopMatrix();
	
	//Tree Layer 3
	glColor3f(0.13, 0.29, 0.);			
	glPushMatrix();
		glTranslatef(-5, 10.5, 0);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(1.792, 2.048, 20, 20);
	glPopMatrix();



}

//--Mouse Click---------------------------------------------------------
void mouse(int b, int s, int x, int y)
{
	if(b == GLUT_LEFT_BUTTON && s == GLUT_DOWN && tree_down == 2)
	{  
		if (tree5Click_x == 1){
			tree5Click_x = -1;
			tree5Click_y = -1;
			tree5Click_z = -1;
			
		}
		else if (tree4Click_x == 1){
			tree4Click_x = -1;
			tree4Click_y = -1;
			tree4Click_z = -1;
		}
		
		else if (tree3Click_x == 1)
		{
			tree3Click_x = -1;
			tree3Click_y = -1;
			tree3Click_z = -1;
		}
		else if (tree2Click_x == 1)
		{
			tree2Click_x = -1;
			tree2Click_y = -1;
			tree2Click_z = -1;
		}
		else 
		{
			tree1Click_x = -1;
			tree1Click_y = -1;
			tree1Click_z = -1;
			tree_down = 3;
		}
	}

	glutPostRedisplay();    
}

//--Timer---------------------------------------------------------------
void FlowerTimer(int value){
	if (dance_flag ==1) {
		if (flower_dance == 1) {
			flower_dance = 2;
			}
		else if (flower_dance == 2) {
			flower_dance = 4;
			}
		else if (flower_dance == 4) {
			flower_dance = 3;
			}
		else{
			flower_dance = 1;
			}
		}
	if (dance_flag == 3) {
		if (flower_dance == 1) {
			flower_dance = 2;
			left_antenna = 60;
			left_bulb = 8.8;
			}
		else if (flower_dance == 2) {
			flower_dance = 4;
			left_antenna = 30;
			left_bulb = 8.5;
			}
		else if (flower_dance == 4) {
			flower_dance = 3;
			right_antenna = -60;
			right_bulb = 8.9;
			}
		else{
			flower_dance = 1;
			right_antenna = 30;
			right_bulb = 7.5;
			}
			 
		}
	glutPostRedisplay();
	glutTimerFunc(300, FlowerTimer, 0);
}

//--Timer---------------------------------------------------------------
void RobotTimer(int value){
	if (robot_mark == 1) {
		if (robot_z < -30) {
			robot_z++;
		}
		else {
			robot_mark = 2;
		}
	}
		
	else if (robot_mark == 2) {
		if (robot_angle < 87) {
			robot_angle+=5;
		}
		else{
			robot_mark = 3;
		}
	}

	else if (robot_mark == 3) {				
		if (robot_x < 9) {
			robot_x++;
		}
		else {
			robot_mark = 4;
		}
	}
	else if (robot_mark == 4) {
		if (robot_z < -18) {
			robot_z++;
		}
		else {
			robot_mark = 5;
		}
	}
	else if (robot_mark == 5){
		if (robot_angle > 0) {
			robot_angle-=5;
		}
		else {
			robot_mark = 6;
		}
	}
	else if (robot_mark == 6) {
		if (robot_z < -4) {
			robot_z++;
		}
		else{
			robot_mark = 7;
		}
	}

	else if (robot_mark == 7) {
		if (robot_angle > -60) {
			robot_angle-=5;
			cam_angle+=3;
		}
		else{
			robot_mark = 8;
		}
	}

	else if (robot_mark == 8) {
		if (left_antenna != -30) {
			left_antenna -= 5;
			left_bulb -= 0.1;
			right_antenna += 8.75;
			right_bulb -= 0.15;
			eyes_red = 0.2;
			eyes_green =  0.2;
			eyes_blue = 0.99;
		} 
		
		else {
			robot_mark = 9;
			glutTimerFunc(1000, FlowerTimer, 0);
		}
	}
	else if (robot_mark == 9)
	{
		if (cam_angle > 0 ) {
			cam_angle-=3;
		}
		else{
			robot_mark = 10;
		}
	}

	else if (robot_mark == 10)
	{
		if (robot_angle < 0) {
			robot_angle+=5;
			if (cam_z < 15) {
				cam_z+=0.5;
			}
		}
		else{
			robot_mark = 11;
		}
	}
	else if (robot_mark == 11)
	{
			if (cam_angle < 80) {
				cam_angle+=8;
				if (robot_z < 80) {
				robot_z+=0.5;
			}
		}
		else{
			robot_mark = 12;
		}
	}
	
	else if (robot_mark == 12)
	{
			if (cam_angle < 120) {
				cam_angle+=8;
				if (robot_angle < 90) {
					robot_angle+=10;
			}
		}
		else{
			robot_mark = 13;
		}
	}
	else if (robot_mark == 13)
	{
			if (cam_angle < 190) {
				cam_angle+=5;
				if (robot_angle > -45) {
					robot_angle-=10;
			}
		}
		else{
			robot_mark = 14;
		}
	}
	else if (robot_mark == 14) {
		if (left_antenna != 10) {
			left_antenna += 5;
			left_bulb += 0.1;
			right_antenna -= 8.75;
			right_bulb += 0.15;
			eyes_red = 0.99;
			eyes_green =  0.1;
			eyes_blue = 0.1;
		} 
		else {
			robot_mark = 15;
		}
	}

	else if (robot_mark == 15) {
		if (cam_angle < 220) {
			cam_angle+= 10.5;	
		}
		else{
		robot_mark = 16;
		}
	}

	else if (robot_mark == 16) {
		if (robot_x > -10) {
			robot_x -= 1;
			robot_z += 1;
			if (cam_x > -8){
				cam_x -= 0.43;
			}
			if (cam_z > -14){
				cam_z -= 1.6;
			}
			if (cam_hgt < 13){
				cam_hgt += 0.16;
			}
		}
		else {
			tree_down = 2;
			robot_mark=17;
		}
	}
	else if (robot_mark == 17 && tree_down == 3) {
		if (robot_x > -18) {
			robot_x -= 1;
			robot_z += 1;
		}
		else {
			robot_mark=18;
		}
	
	}
	else if (robot_mark == 18) {
		if (robot_angle > -65) {
			robot_angle-=2;
		}
		else {
			robot_mark=19;
		}
	}
	
	else if (robot_mark == 19) {
	 	if (colour_change == 1) {
			rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
	 		colour_change++;
	 	}
	 	else if (colour_change == 1) {
	 		rbulb_red = 0.584;
			rbulb_green = 0.588;
			rbulb_blue = 0.572;
			
			lbulb_red = 0.64;
			lbulb_green = 0.0;
			lbulb_blue = 1.0;

	 		colour_change++;
	 	}
	 	else if (colour_change == 2) {
	 		rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
			
			lbulb_red = 0.584;
			lbulb_green = 0.588;
			lbulb_blue = 0.572;
			
			colour_change++;

	 	}
	 	else if (colour_change == 3) {
	 		rbulb_red = 0.584;
			rbulb_green = 0.588;
			rbulb_blue = 0.572;
			
			lbulb_red = 0.64;
			lbulb_green = 0.0;
			lbulb_blue = 1.0;

	 		
	 		colour_change++;
	 	}
	 	else if (colour_change == 4) {
	 		rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
			
			lbulb_red = 0.584;
			lbulb_green = 0.588;
			lbulb_blue = 0.572;

	 		colour_change++;
	 	}
	 		else if (colour_change == 5) {
	 		rbulb_red = 0.584;
			rbulb_green = 0.588;
			rbulb_blue = 0.572;
			
			lbulb_red = 0.64;
			lbulb_green = 0.0;
			lbulb_blue = 1.0;

	 		
	 		colour_change++;
	 	}
	 	else if (colour_change == 6) {
	 		rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
			
			lbulb_red = 0.584;
			lbulb_green = 0.588;
			lbulb_blue = 0.572;

	 		colour_change++;
	 	}
	 		else if (colour_change == 7) {
	 		rbulb_red = 0.584;
			rbulb_green = 0.588;
			rbulb_blue = 0.572;
			
			lbulb_red = 0.64;
			lbulb_green = 0.0;
			lbulb_blue = 1.0;

	 		
	 		colour_change++;
	 	}
	 	else if (colour_change == 8) {
	 		rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
			
			lbulb_red = 0.584;
			lbulb_green = 0.588;
			lbulb_blue = 0.572;

	 		colour_change++;
	 	}
	 		else if (colour_change == 9) {
	 		rbulb_red = 0.584;
			rbulb_green = 0.588;
			rbulb_blue = 0.572;
			
			lbulb_red = 0.64;
			lbulb_green = 0.0;
			lbulb_blue = 1.0;

	 		
	 		colour_change++;
	 	}
	 	else if (colour_change == 10) {
	 		rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
			
			lbulb_red = 0.584;
			lbulb_green = 0.588;
			lbulb_blue = 0.572;

	 		colour_change++;
	 	}
	 		else if (colour_change == 11) {
	 		rbulb_red = 0.584;
			rbulb_green = 0.588;
			rbulb_blue = 0.572;
			
			lbulb_red = 0.64;
			lbulb_green = 0.0;
			lbulb_blue = 1.0;
			
			flower_dance = 1;
			dance_flag = 2;
	 		
	 		colour_change++;
	 	}
	 	else if (colour_change == 12) {
	 		rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
			
			lbulb_red = 0.584;
			lbulb_green = 0.588;
			lbulb_blue = 0.572;
			
			flower_y+= 0.3;
	 		colour_change++;
	 	}
	 		else if (colour_change == 13) {
	 		rbulb_red = 0.584;
			rbulb_green = 0.588;
			rbulb_blue = 0.572;
			
			lbulb_red = 0.64;
			lbulb_green = 0.0;
			lbulb_blue = 1.0;

	 		flower_y+= 0.3;
	 		colour_change++;
	 	}
	 	else if (colour_change == 14) {
	 		rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
			
			lbulb_red = 0.584;
			lbulb_green = 0.588;
			lbulb_blue = 0.572;
			
			flower_y+= 0.3;
	 		colour_change++;
	 	}

	 	else if (colour_change == 15) {
	 		rbulb_red = 0.64;
			rbulb_green = 0.0;
			rbulb_blue = 1.0;
			
			lbulb_red = 0.64;
			lbulb_green = 0.0;
			lbulb_blue = 1.0;

			flower_y+= 0.3;
	 		colour_change++;
	 	}
		else{
			robot_mark=20;
		}
	}
	else if (robot_mark == 20) {
			if(robot_angle > -180) {
				robot_angle-=4;
				flower_angle -=2;
				if (flower_x < 0.04) { 
					flower_x -= 0.01; 
				}
				else{
					flower_x +=0.01;
				}
		}
		else {
			
			robot_mark = 21;
		}
	}
	else if (robot_mark == 21) {
		 if (robot_x < 0) {
			robot_x += 1;
			flower_x += 1.6;
			robot_z -= 1;
			flower_z -= 1.58;
			flower_y += .05;
			if (cam_x < 0){
				cam_x += 0.43;
			}
			if (cam_z < 15){
				cam_z += 1.6;
			}
			if (cam_hgt > 10){
				cam_hgt -= 0.16;
			}
		}
		else {
			robot_mark=22;
		}
	}
	else if (robot_mark == 22) {
		if (flower_y > 3){
			flower_y--;
		}
		else {
			lbulb_red = 0.584;
			lbulb_green = 0.588;
			lbulb_blue = 0.572;
			
	 		rbulb_red = 0.584;
			rbulb_green = 0.588;
			rbulb_blue = 0.572;
			
			dance_flag = 3;
			robot_mark = 23;
		} 
		

}
	glutPostRedisplay();
	glutTimerFunc(200, RobotTimer, 0);
}

void camera()
{
	dir_x = -sin(xz_angle), dir_y = tan(yz_angle),  dir_z = -cos(xz_angle);
	
	
	lookpoint_x = cam_x + 2 * dir_x;
	lookpoint_y = cam_hgt + 2 * dir_y;
	lookpoint_z = cam_z + 2 * dir_z;
	
	gluLookAt(cam_x, cam_hgt, cam_z, lookpoint_x, lookpoint_y, lookpoint_z, 0., 1., 0.);	
}


//--Display: -----------------------------------------------------------
void display()  
{
	
	// Light source position = (lx, ly, lz)
	float lx = 10.;
	float ly = 10.;
	float lz = 10.;
	
	float shadowMat[16] = { ly,0,0,0, -lx,0,-lz,-1,0,0,ly,0,  0,0,0,ly };
	
	float lpos[4] = {lx, ly, lz, 1.0};  //light's position
	float spot1_dir[] = {-50.f,-10.f,0.f};
	float spot2_dir[] = {50.f,-10.f,0.f};
	float lgt1_pos[] = {0.f, 7.9f, 0, 1.0f};
	float lgt2_pos[] = {0.f, 7.9f, 0, 1.0f};




	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera();
	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position
	
    glRotatef(cam_angle, 0.0, 1.0, 0.0);//Rotate the scene about the y-axis  
	
	
	
	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
	skybox();
	glEnable(GL_LIGHTING);	       //Enable lighting when drawing the model
	
 
	//Forest
	for (tree_x = 44.8; tree_x > -38; tree_x = tree_x-6) {
		glPushMatrix();
			glTranslatef(tree_x, 0, -43);
			drawTree();
		glPopMatrix();
		
		tree_z += 20;
		tree_x += 1;
		
		glPushMatrix();
			glTranslatef(tree_x, 0, tree_z);
			drawTree();
		glPopMatrix();
		
		tree_z -=5;
		tree_x += 1;	
		
		glPushMatrix();
			glTranslatef(tree_x, 0, tree_z);
			drawTree();
		glPopMatrix();
		
		tree_z -=10;	
		tree_x -= 3;
		
		glPushMatrix();
			glTranslatef(tree_x, 0, tree_z);
			drawTree();
		glPopMatrix();
		
		tree_z -= 5;
	}
	
	//Robot Postions
	glPushMatrix();
		glTranslatef(robot_x, robot_y, robot_z);
		glRotatef(robot_angle, 0, 1, 0);
		glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot1_dir);
		glLightfv(GL_LIGHT2, GL_POSITION, lgt2_pos);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot2_dir);
		drawRobotSilhouette();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(robot_x, robot_y, robot_z);
		glRotatef(robot_angle, 0, 1, 0);
		drawRobotAntennas();
	glPopMatrix();
	
	// Disappearing Trees
	glPushMatrix();
		glTranslatef(-15, 0, 30);
		glScalef(tree1Click_x,tree1Click_y,tree1Click_z);
		drawTree();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-18.5, 0, 25);
		glScalef(tree2Click_x,tree2Click_y,tree2Click_z);
		drawTree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20, 0, 20);
		glScalef(tree3Click_x,tree3Click_y,tree3Click_z);
		drawTree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30, 0, 25);
		glScalef(tree4Click_x,tree4Click_y,tree4Click_z);
		drawTree();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-38.5, 0, 30);
		glScalef(tree5Click_x,tree5Click_y,tree5Click_z);
		drawTree();
	glPopMatrix();
	
	// Flower Postion
	glPushMatrix();
		if (dance_flag == 1) {
			flower_y = -0.1;
		}
		if (dance_flag == 3) {
			flower_y = 2;
		}
		glTranslatef(flower_x, flower_y, flower_z);
		glRotatef(flower_angle, 0, 1, 0);
		glScalef(.7,.7,.7);
		drawFlowerStem();
	glPopMatrix();
	
	if (flower_dance == 1) {
		glPushMatrix();
			if (dance_flag == 1) {
				flower_y = 0;
			}
			if (dance_flag == 3) {
				flower_y = 2;
			}
			glTranslatef(flower_x, flower_y, flower_z);
			glRotatef(flower_angle, 0, 1, 0);
			glScalef(.7,.7,.7);
			drawFlower();
		glPopMatrix();
	}

	else if (flower_dance == 2) {
		glPushMatrix();
			if (dance_flag == 1) {
				flower_y = 2;
				glTranslatef(-27.2, flower_y, 30);
			}
			if (dance_flag == 3) {
				flower_y = 4.3;
				glTranslatef(3.2, flower_y, 0);
			}
			glScalef(.7,.7,.7);
			glRotatef(75, 0, 0, 1);
			drawFlower();
		glPopMatrix();
	}
	
	else if (flower_dance == 3) {
		glPushMatrix();
			if (dance_flag == 1) {
				flower_y = 2;
				glTranslatef(-32.8,flower_y, 30);
			}
			if (dance_flag == 3) {
				flower_y = 4.3;
				glTranslatef(-3.1,flower_y, 0);
			}
			glScalef(.7,.7,.7);
			glRotatef(-75, 0, 0, 1);
			drawFlower();
		glPopMatrix();
	}
	else if (flower_dance == 4) {
		glPushMatrix();
		if (dance_flag == 1){
			flower_y = 0;
		}
		if (dance_flag == 3){
			flower_y = 2;
		}
			glTranslatef(flower_x, flower_y, flower_z);
			glScalef(.7,.7,.7);
			drawFlower();
		glPopMatrix();
	}

	//Static Trees
	glPushMatrix();
		glTranslatef(40, 0, 30);
		drawTree();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(40, 0, 25);
		drawTree();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(35, 0, 35);
		drawTree();
	glPopMatrix();

	//Stand
	glPushMatrix();
		glColor3f(0.7, 0.6, 0.36);
		drawStand();
	glPopMatrix();
	
	//  Draw shadow
	glDisable(GL_LIGHTING);
	glPushMatrix();     
		glMultMatrixf(shadowMat);
		glColor4f(0.2, 0.2, 0.2, 1.0);
		glTranslatef(0, 2., 0);
		drawStand();
	glPopMatrix();
	
	//pot Plant
	glPushMatrix();
		glScalef(2.,2.,2.);
		glTranslatef(0, -1.3, 0);
		drawPotPlant();
	glPopMatrix();
	
	glutSwapBuffers();
}

//--Initialize OpenGL parameters ---------------------------------------
void initialize()
{
	loadTexture();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour
	float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 0.1};
	q = gluNewQuadric();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    //Spotlight 1
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);
    
    //Spotlight 2
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,0.01);
     
     
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50); 
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    gluQuadricDrawStyle(q, GLU_FILL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000.0);   //Camera Frustum
}

//--Special key event callback -----------------------------------------
void special(int key, int x, int y)
{
	if(cam_hgt <=35){
		if(yz_angle <= 0.1) yz_angle = 0.1;
	}
	
	if(key==GLUT_KEY_LEFT) xz_angle += 0.1;           //Rotate camera left
	else if(key==GLUT_KEY_RIGHT) xz_angle -= 0.1;	 //Rotate camera Right
	else if(key==GLUT_KEY_UP) yz_angle += 0.1;      //Rotate camera up
	else if(key==GLUT_KEY_DOWN) yz_angle -= 0.1;   //Rotate camera Down
	
	if(yz_angle <= -1.5) yz_angle = -1.5;			//Clamping Minimum yz Angle
	if(yz_angle >= 1.5) yz_angle = 1.5;		//Clamping Maximum yz Angle

	glutPostRedisplay();

}

//--Main: Initialize glut window and register call backs ---------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   int screenHeight;
   screenHeight = glutGet (GLUT_SCREEN_HEIGHT);
   glutInitWindowSize (screenHeight, screenHeight);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Robot World");
   initialize();
   glutTimerFunc(1000, RobotTimer, 0);
   glutDisplayFunc(display);
   glutSpecialFunc(special); 
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;
}
