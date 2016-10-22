#ifndef PHYSXINIT_H
#define PHYSXINIT_H
#include "PhysicsBase.h"
#ifdef _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#else
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#endif

#include <PxPhysicsAPI.h>
#include <PxDefaultErrorCallback.h>
#include <PxDefaultAllocator.h> 
#include <PxDefaultCpuDispatcher.h>

class CPhysXInit :public CPhysicsBase {
public:
	CPhysXInit(void);
	~CPhysXInit(void);
	virtual void CreateGround();
	virtual void CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height);
	virtual void CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height, CObjectBase::materialType);
	virtual void CreateConvex(GLMmodel &objmodel_ptr, float start_pos_x, float start_pos_y, float start_pos_z, float size_x, float size_y, float size_z);
	virtual void CreateConvex(GLMmodel &objmodel_ptr, float start_pos_x, float start_pos_y, float start_pos_z, float size_x, float size_y, float size_z, CObjectBase::materialType);
	virtual void CreateBox(float start_pos_x, float start_pos_y, float start_pos_z, float size_x, float size_y, float size_z);
	virtual void CreateBox(float start_pos_x, float start_pos_y, float start_pos_z, float size_x, float size_y, float size_z, CObjectBase::materialType);
	virtual void CreateSphere(float x_position, float y_position, float z_position, float radius);
	virtual void CreateSphere(float x_position, float y_position, float z_position, float radius, CObjectBase::materialType material_type);
	virtual void DeleteAllObjects();
	virtual void Initialize();
	virtual void SetGravity(float set_gravity_x, float set_gravity_y, float set_gravity_z);
	virtual void StepSim(float time_step);
	virtual void RenderAllObjects();
	virtual void StopSim();
	virtual int GetNumberOfObjects();
	std::vector <CObjectBase*> allObjects; //an std::vector to hold all the objects of this engine
private:
	void SetMatrix(physx::PxMat33 m, physx::PxVec3 t, float *mat);
	void ObjectProperties(CObjectBase::materialType _matType, int);
	void MaterialSetter(CObjectBase::materialType _matType, physx::PxMaterial* & material, CObjectBase & tempBoxHelper);
	float GetDensity(CObjectBase::materialType materialType);
	std::vector <physx::PxRigidBody*>physXObjects; //the old "actors"
	physx::PxScene* myScene;
	physx::PxPhysics* myPhysicsSDK;
	physx::PxFoundation* myFoundation;             
	physx::PxDefaultCpuDispatcher* myCpuDispatcher;
	physx::PxSimulationFilterShader myDefaultFilterShader;	
	physx::PxDefaultErrorCallback myDefaultErrorCallback;
	physx::PxDefaultAllocator myDefaultAllocatorCallback;
	physx::PxRigidStatic* myGroundPlane; //a static obect that will hold the ground
	physx::PxShape* myGroundShape; //the ground shape collider
};
#endif 
