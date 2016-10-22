#ifndef SPHERE_H
#define SPHERE_H
#include "ObjectBase.h"
class CSphere : public CObjectBase {
public:
	CSphere(void);
	~CSphere(void);
	virtual void DrawSelf(); //see Box.cpp
	virtual void SetDrawingRot(float _xRot, float _yRot, float _zRot){}; //see Box.cpp
};
#endif 
