#include "ObjectBase.h"

void CObjectBase::SetSize(float _newWidth, float _newHeight, float _newLength)
{
	width = _newWidth;
	height = _newHeight;
	length = _newLength;
}

void CObjectBase::SetMaterial(materialType _newMatType)
{
	myMaterial = _newMatType;
}

void CObjectBase::ChangeDrawingColor()
{
	switch (myMaterial)
	{ 
	case CONCRETE: //concrete, gray
		glColor3f(0.5f,0.5f,0.5f);
	break;
	case RUBBER: //rubber, blueish
		glColor3f(0.5f,0.5f,0.9f);
	break;
	case DEFFAULT: //pink, similar to the default shader color in unity. When something is in pink means that there's a problem.
		glColor3f(1.0f,0.0f,1.0f);
	break;
	default:
		glColor3f(1.0f,1.0f,0.0f);//the deffault of the switch. If an object is yellow this means that something's wrong with "myMaterial"
	break;
	}
}



