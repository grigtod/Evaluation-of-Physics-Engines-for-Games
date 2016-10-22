#ifndef BULLETINIT_H
#define BULLETINIT_H
#include "PhysicsBase.h"
#include <btBulletDynamicsCommon.h>

class CBulletInit :
	public CPhysicsBase
{
public:
	CBulletInit(void);
	~CBulletInit(void);
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
	std::vector <CObjectBase*> allObjects; 
private:
	float ReturnMassForMaterial(CObjectBase::materialType _matType);
	void AdjustConstructionInfo(btRigidBody::btRigidBodyConstructionInfo &tempRigidBodyCI, CObjectBase &tempObject, CObjectBase::materialType _matType);
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray <btRigidBody*> bulletObjects;//notce the use of a custom vector-ish object type instead of the stl vector, because of allignment issues
	
};
#endif
