#ifndef HAVOKINIT_H
#define HAVOKINIT_H
#include "PhysicsBase.h"
//http://marcoarena.wordpress.com/2011/04/17/nice-to-meet-you-havok/ //includes are seen form here
#include <Common/Base/hkBase.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Config/hkConfigVersion.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Container/String/hkStringBuf.h>
//convex hull
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics2012/Collide/Shape/Misc/Bv/hkpBvShape.h>
#include <Physics2012/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics2012/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics2012/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics2012/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h> 
#include <Physics2012/Collide/Shape/Convex/hkpConvexShape.h>
//convex hull end
#include <Physics2012/Collide/hkpCollide.h>
#include <Physics2012/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics2012/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <Common/Base/System/Init/PlatformInit.cxx>

class CHavokInit : public CPhysicsBase {
public:
	CHavokInit(void);
	~CHavokInit(void);
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
	void SetMaterialProperties(CObjectBase & tempObject, CObjectBase::materialType tempMat, hkReal &tempMass);
	void SetProperMatrix(int i);
	hkArray <hkpRigidBody*> havokObjects; 
	hkMemoryRouter* memoryRouter;
	hkpWorld* physicsWorld;
};
#endif
