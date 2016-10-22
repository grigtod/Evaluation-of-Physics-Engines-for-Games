#include "Box.h"

CBox::CBox(void)
{
	SetMaterial(DEFFAULT);
}

CBox::~CBox(void)
{
}

void CBox::DrawSelf()
{
	glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		ChangeDrawingColor();//implementation in the parent class
		glMultMatrixf(myMatrix);//set the drawing matrix
		glutSolidCube(1.0f);//draw a solid cude. Currently the size of the cube cannot be changed
		glColor3f(0.0f,0.0f,0.0f); 
		glutWireCube(1.01f);//draw the wireframe of the cube, a bit bigget from it, so ir is visible
	glPopMatrix();
}

