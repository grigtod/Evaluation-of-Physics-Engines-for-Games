#include "Convex.h"

CConvex::CConvex(GLMmodel & mesh)
{
	SetMaterial(DEFFAULT);
	modelMesh = &mesh;
	glmUnitize(modelMesh);
    glmFacetNormals(modelMesh);
    glmVertexNormals(modelMesh, 10);
}

CConvex::~CConvex(void)
{
	delete modelMesh;
}

void CConvex::DrawSelf()
{
	glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
		ChangeDrawingColor();
		glMultMatrixf(myMatrix);
		glmDraw(modelMesh, GLM_SMOOTH);// | GLM_MATERIAL);
	glPopMatrix();
}

