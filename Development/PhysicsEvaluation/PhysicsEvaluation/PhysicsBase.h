#ifndef PHYSICSBASE_H
#define PHYSICSBASE_H
#include <vector> //used in all the inheriting classes from this base class
#include "Box.h"
#include "Sphere.h"
#include "Capsule.h"
#include "Convex.h"
#include "glm.h" //used for opening/rendering meshes. reference: http://www.d.umn.edu/~ddunham/cs5721f07/schedule/resources/code/glm.h
class CPhysicsBase
{
public: // all the functions are abstract. This class is interface used as a base class for all physics classes!
	 virtual void CreateBox(float start_pos_x, float start_pos_y, float start_pos_z, float size_x, float size_y, float size_z, CObjectBase::materialType)=0; //creates a box
	 virtual void CreateSphere(float start_pos_x, float start_pos_y, float start_pos_z, float size_x, CObjectBase::materialType)=0; //creates a sphere 
	 virtual void CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height, CObjectBase::materialType)=0;//creates a capsule
	 virtual void CreateConvex(GLMmodel &objmodel_ptr, float start_pos_x, float start_pos_y, float start_pos_z, float size_x, float size_y, float size_z, CObjectBase::materialType)=0;//creates a convex
	 virtual void CreateGround()=0; //cretes the ground 
	 virtual void DeleteAllObjects()=0; //deletes all the created objects
	 virtual void SetGravity(float set_gravity_x, float set_gravity_y, float set_gravity_z)=0; //set the gravity of the simulation.
	 virtual void StepSim(float time_step)=0; //steps the simulation, given a timestep
	 virtual void StopSim()=0; //this class will be called to stop/start simulation. This feature is currently not implemented
	 virtual void RenderAllObjects()=0; //this class is called to render all objects of the physics engine
	 virtual void Initialize()=0; //initialization of the engine
	 virtual int GetNumberOfObjects() = 0; //returns how many objects are created in that physics engine
};
#endif 
