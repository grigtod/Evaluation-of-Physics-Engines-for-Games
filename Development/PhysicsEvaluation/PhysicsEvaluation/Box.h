#ifndef BOX_H
#define BOX_H
#include "ObjectBase.h"
class CBox : public CObjectBase {
public:
	CBox(void);
	~CBox(void);
	virtual void DrawSelf();//when this function is called the object is rendered
	virtual void SetDrawingRot(float _xRot, float _yRot, float _zRot){}; //no implementation for the box
};
#endif 
