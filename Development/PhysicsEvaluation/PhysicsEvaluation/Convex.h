#ifndef CONVEX_H
#define CONVEX_H
#include "ObjectBase.h"
#include "glm.h"
//mesh rendering and loading uses a third party library that can be found : http://www.d.umn.edu/~ddunham/cs5721f07/schedule/resources/lab_opengl07.html
class CConvex : public CObjectBase {
public:
	//GLMmodel *objmodel_ptr;
	GLMmodel * modelMesh;
	CConvex(GLMmodel & mesh);//the constructor receives already loaded mesh. This is done for optimization reasons, so the same mesh won't be loaded every time a new one is created.
	~CConvex(void);
	virtual void DrawSelf();
	virtual void SetDrawingRot(float _xRot, float _yRot, float _zRot){};
};
#endif 
