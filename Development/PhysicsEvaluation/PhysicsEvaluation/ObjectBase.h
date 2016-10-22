#ifndef OBJECTBASE_H
#define OBJECTBASE_H
#include <iostream>
#include <GL/freeglut.h>
class CObjectBase {
public: //this class is not an interface, because some functions have implementation here...
	enum materialType {DEFFAULT, CONCRETE, RUBBER }; // enumeration of the 3 types of materials
	virtual void DrawSelf()=0; //abstract function that renders the object using freeglut
	virtual void SetDrawingRot(float, float, float)=0; //set the drawing rotation. Currently only used for the capsules
	void SetSize(float, float, float); //set the size of the object
	void SetMaterial(materialType); //set a material for the object
	float myMatrix[16]; //the matrix of the object
	materialType myMaterial;  //the material of the object
protected:
	void ChangeDrawingColor(); //change the drawing color of an object(according to the physics material)
	float width,height,length; //the width,height and length of the object
};
#endif 
