#include "Capsule.h"

CCapsule::CCapsule(void)
{
	SetMaterial(DEFFAULT);
	xRot = 0.0f;//capsules have an implementation of the virtual SetDrawingRot
	yRot = 0.0f;
	zRot = 0.0f;
}

CCapsule::~CCapsule(void)
{
}

void CCapsule::DrawSelf()
{
	glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		glMultMatrixf(myMatrix);
		glRotatef(xRot,1,0,0);//offset the rendering of the capusle
		glRotatef(yRot,0,1,0);
		glRotatef(zRot,0,0,1);
		ChangeDrawingColor();
		glutSolidCylinder(length,height,10,3);//freeglut does not have implemented capsule drawing
		glColor3f(0.0f,0.0f,0.0f);
		glutWireCylinder(length+0.01,height+0.01,10,3);//but a capsule is merely a cylinder with 2 spheres on both sides
		ChangeDrawingColor();
		glutSolidSphere(length, 10, 10);
		glColor3f(0.0f,0.0f,0.0f);
		glutWireSphere(length+0.01, 10, 10);//(facepalm)
		glTranslatef(0,0,height);
		ChangeDrawingColor();
		glutSolidSphere(length, 10, 10);
		glColor3f(0.0f,0.0f,0.0f);
		glutWireSphere(length+0.01, 10, 10);
	glPopMatrix();
}

void CCapsule::SetDrawingRot(float _xRot, float _yRot, float _zRot)
{
	xRot = _xRot;
	yRot = _yRot;
	zRot = _zRot;
}



