#include "Sphere.h"

CSphere::CSphere(void)
{
	SetMaterial(DEFFAULT);
}

CSphere::~CSphere(void)
{
}

void CSphere::DrawSelf()
{
	glPushMatrix(); //see Box.cpp
		glEnable(GL_COLOR_MATERIAL);
		ChangeDrawingColor();
		glMultMatrixf(myMatrix);
		glutSolidSphere(0.5f,10,10);
		glColor3f(0.0f,0.0f,0.0f);
		glutWireSphere(0.501f,10,10);
	glPopMatrix();
}


