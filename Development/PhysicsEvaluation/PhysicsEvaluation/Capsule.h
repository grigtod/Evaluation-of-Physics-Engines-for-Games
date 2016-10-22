#ifndef CAPSULE_H
#define CAPSULE_H
#include "ObjectBase.h"
class CCapsule : public CObjectBase {
public:
	CCapsule(void);
	~CCapsule(void);
	virtual void DrawSelf();
	virtual void SetDrawingRot(float _xRot, float _yRot, float _zRot);//implementation of the virtual SetDrawingRot
private:
	float xRot;//the rendering rotation offsets
	float yRot;
	float zRot;
};
#endif 
