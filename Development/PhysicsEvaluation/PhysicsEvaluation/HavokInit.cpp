#include "HavokInit.h"

#include <Common/Base/keycode.cxx> //this needs to be placed here. If placed in the header it doesn't work

// This excludes libraries that are not going to be linked
// from the project configuration, even if the keycodes are
// present
#undef HK_FEATURE_PRODUCT_AI 
#undef HK_FEATURE_PRODUCT_ANIMATION 
#undef HK_FEATURE_PRODUCT_CLOTH 
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012 
#undef HK_FEATURE_PRODUCT_DESTRUCTION 
#undef HK_FEATURE_PRODUCT_BEHAVIOR 
#undef HK_FEATURE_PRODUCT_MILSIM 
#undef HK_FEATURE_PRODUCT_PHYSICS
#define HK_EXCLUDE_LIBRARY_hkpVehicle
#define HK_EXCLUDE_LIBRARY_hkCompat
#define HK_EXCLUDE_LIBRARY_hkSceneData
#define HK_EXCLUDE_LIBRARY_hkcdCollide
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches 
#define HK_EXCLUDE_FEATURE_hkpHeightField
#define HK_EXCLUDE_FEATURE_hkpAccurateInertiaTensorComputer
#define HK_EXCLUDE_FEATURE_hkpUtilities
#define HK_EXCLUDE_FEATURE_hkpVehicle
#define HK_EXCLUDE_FEATURE_hkpCompressedMeshShape
#define HK_EXCLUDE_FEATURE_hkpConvexPieceMeshShape
#define HK_EXCLUDE_FEATURE_hkpExtendedMeshShape
#define HK_EXCLUDE_FEATURE_hkpMeshShape
#define HK_EXCLUDE_FEATURE_hkpSimpleMeshShape
#define HK_EXCLUDE_FEATURE_hkpPoweredChainData
#define HK_EXCLUDE_FEATURE_hkMonitorStream
#include <Common/Base/Config/hkProductFeatures.cxx>

static void HK_CALL errorReport(const char* msg, void*)
{ 
	//this function can be used to send messages to the console. It is needed for havok to funtion
}

CHavokInit::CHavokInit(void)
{
	Initialize();
}

CHavokInit::~CHavokInit(void)
{
	DeleteAllObjects();
	delete physicsWorld;
	delete memoryRouter;
}

void CHavokInit::CreateGround() {// Create the floor as a fixed box
	hkpRigidBodyCinfo boxInfo;
	hkVector4 boxSize(300.0f, 20.0f , 300.0f);
	boxInfo.m_shape = new hkpBoxShape(boxSize);
	boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
	boxInfo.m_position.set(0.0f, -20.0f, 0.0f);
	boxInfo.m_restitution = 0.1f;
	physicsWorld->addEntity(new hkpRigidBody(boxInfo));
}
void CHavokInit::SetMaterialProperties(CObjectBase & tempObject, CObjectBase::materialType tempMat, hkReal &tempMass){
	switch(tempMat)
	{
	case CObjectBase::CONCRETE: 
		tempObject.myMaterial = CObjectBase::CONCRETE;
		tempMass = 10.0f;
	break;
	case CObjectBase::RUBBER: 
		tempObject.myMaterial = CObjectBase::RUBBER;
		tempMass = 0.5f;
	break;
	case CObjectBase::DEFFAULT: //change these...
		tempObject.myMaterial = CObjectBase::DEFFAULT;
		tempMass = 3.0f;
	break;
	}
}

void CHavokInit::CreateBox(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ) {
	
	CHavokInit::CreateBox(_spawnPosX,_spawnPosY,_spawnPosZ,_sizeX,_sizeY,_sizeZ, CObjectBase::DEFFAULT);
}

void CHavokInit::CreateBox(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ, CObjectBase::materialType _matType) 
{
	hkpRigidBodyCinfo boxInfo; //construction info
	hkVector4 half(_sizeX/2,_sizeY/2,_sizeZ/2);//half of the box...
	boxInfo.m_shape = new hkpBoxShape(half); //set up the construction info
	boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
	allObjects.push_back(new CBox());
	hkReal boxMass;
	SetMaterialProperties(*allObjects[allObjects.size()-1], _matType,boxMass );
	hkMassProperties boxeMassProperties;
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(half,boxMass,boxeMassProperties);
	boxInfo.m_inertiaTensor = boxeMassProperties.m_inertiaTensor;
	boxInfo.m_centerOfMass = boxeMassProperties.m_centerOfMass;
	boxInfo.m_mass = boxeMassProperties.m_mass;
	havokObjects.pushBack(new hkpRigidBody(boxInfo));
	havokObjects[havokObjects.getSize()-1]->setPositionAndRotation(hkVector4(_spawnPosX, _spawnPosY, _spawnPosZ),hkQuaternion::getIdentity());
	havokObjects[havokObjects.getSize()-1]->setLinearVelocity(hkVector4::getZero());
	havokObjects[havokObjects.getSize()-1]->setAngularVelocity(hkVector4::getZero());
	physicsWorld->addEntity(havokObjects[havokObjects.getSize()-1]);
}

void CHavokInit::CreateSphere(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _radius) {
	CHavokInit::CreateSphere(_spawnPosX,_spawnPosY,_spawnPosZ,_radius, CObjectBase::DEFFAULT);
}

void CHavokInit::CreateSphere(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _radius, CBox::materialType _matType) 
{
	hkReal sphereRadius = _radius/2;
	hkpRigidBodyCinfo sphereInfo;
	sphereInfo.m_shape = new hkpSphereShape(sphereRadius);;
	sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
	hkReal sphereMass;
	allObjects.push_back(new CSphere());
	SetMaterialProperties(*allObjects[allObjects.size()-1], _matType,sphereMass );
	hkMassProperties sphereMassProperties; //compute the properties of the sphere
	hkpInertiaTensorComputer::computeSphereVolumeMassProperties(sphereRadius,sphereMass,sphereMassProperties);
	sphereInfo.m_inertiaTensor = sphereMassProperties.m_inertiaTensor;
	sphereInfo.m_centerOfMass = sphereMassProperties.m_centerOfMass;
	sphereInfo.m_mass = sphereMassProperties.m_mass;
	havokObjects.pushBack(new hkpRigidBody(sphereInfo));
	havokObjects[havokObjects.getSize()-1]->setPositionAndRotation(hkVector4(_spawnPosX, _spawnPosY, _spawnPosZ),hkQuaternion::getIdentity());
	havokObjects[havokObjects.getSize()-1]->setLinearVelocity(hkVector4::getZero());
	havokObjects[havokObjects.getSize()-1]->setAngularVelocity(hkVector4::getZero());
	physicsWorld->addEntity(havokObjects[havokObjects.getSize()-1]);
}

void CHavokInit::CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height) {
	CHavokInit::CreateCapsule(start_pos_x, start_pos_y,start_pos_z,radius,height, CObjectBase::DEFFAULT);
}

void CHavokInit::CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height, CObjectBase::materialType _matType) 
{
	hkVector4 capDown(-height,0,0); //set up the 2 caps of the capsule
    hkVector4 capUp(height,0,0);
	hkpRigidBodyCinfo capsuleInfo;
	capsuleInfo.m_shape = new hkpCapsuleShape(capDown,capUp, radius);
	capsuleInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;//no capsule inertia available
	hkReal boxMass;
	allObjects.push_back(new CCapsule());
	allObjects[allObjects.size()-1]->SetSize(radius,height,radius); //set the size of the capsule object to be draw
	allObjects[allObjects.size()-1]->SetDrawingRot(0,90,0);//set the rotation offset. Each engine creates capsules with different rotation...
	SetMaterialProperties(*allObjects[allObjects.size()-1], _matType,boxMass );
	hkMassProperties capsuleMassProperties;
	hkpInertiaTensorComputer::computeCapsuleVolumeMassProperties(capDown,capUp, radius, boxMass,capsuleMassProperties);
	capsuleInfo.m_inertiaTensor = capsuleMassProperties.m_inertiaTensor;
	capsuleInfo.m_centerOfMass = capsuleMassProperties.m_centerOfMass;
	capsuleInfo.m_mass = capsuleMassProperties.m_mass;
	hkpRigidBody* boxRigidBody = new hkpRigidBody(capsuleInfo);
	havokObjects.pushBack(new hkpRigidBody(capsuleInfo));
	havokObjects[havokObjects.getSize()-1]->setPositionAndRotation(hkVector4(start_pos_x, start_pos_y, start_pos_z),hkQuaternion::getIdentity());
	havokObjects[havokObjects.getSize()-1]->setLinearVelocity(hkVector4::getZero());
	havokObjects[havokObjects.getSize()-1]->setAngularVelocity(hkVector4::getZero());
	physicsWorld->addEntity(havokObjects[havokObjects.getSize()-1]);
}

void CHavokInit::CreateConvex(GLMmodel &objmodel_ptr,float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ) {
	CHavokInit::CreateConvex(objmodel_ptr, _spawnPosX,_spawnPosY,_spawnPosZ,_sizeX,_sizeY,_sizeZ, CObjectBase::DEFFAULT);
}

void CHavokInit::CreateConvex(GLMmodel &objmodel_ptr,float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ, CObjectBase::materialType _matType) 
{
	//the way convex is loaded was seen from convexShapesDemo.cpp in the havok examples
	hkpConvexVerticesShape* convexShape = HK_NULL;
	int numVertices = objmodel_ptr.numvertices;
	int stride = sizeof(hkReal) * 3;
	objmodel_ptr.vertices[0] = objmodel_ptr.vertices[numVertices*3];//adjust the objmodel_ptr, because of the way the bunny.obj is created...
	objmodel_ptr.vertices[1] = objmodel_ptr.vertices[(numVertices*3)+1];
	objmodel_ptr.vertices[2] = objmodel_ptr.vertices[(numVertices*3)+2];
	hkStridedVertices stridedVerts;
	stridedVerts.m_numVertices = numVertices;
	stridedVerts.m_striding = stride;
	stridedVerts.m_vertices = objmodel_ptr.vertices;
	convexShape = new hkpConvexVerticesShape(stridedVerts);
	hkVector4 helf(_sizeX/2,_sizeY/2,_sizeZ/2);
	hkpRigidBodyCinfo meshInfo;
	meshInfo.m_shape = convexShape;
	meshInfo.m_friction = 0.8f;
	meshInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA; //no option for convex/ mesh
	hkReal meshMass;
	allObjects.push_back(new CConvex(objmodel_ptr));
	SetMaterialProperties(*allObjects[allObjects.size()-1], _matType,meshMass );
	hkMassProperties meshMassProperties;
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(helf,meshMass,meshMassProperties);
	meshInfo.m_inertiaTensor = meshMassProperties.m_inertiaTensor;
	meshInfo.m_centerOfMass = meshMassProperties.m_centerOfMass;
	meshInfo.m_mass = meshMassProperties.m_mass;
	havokObjects.pushBack(new hkpRigidBody(meshInfo));
	havokObjects[havokObjects.getSize()-1]->setPositionAndRotation(hkVector4(_spawnPosX, _spawnPosY, _spawnPosZ),hkQuaternion::getIdentity());
	havokObjects[havokObjects.getSize()-1]->setLinearVelocity(hkVector4::getZero());
	havokObjects[havokObjects.getSize()-1]->setAngularVelocity(hkVector4::getZero());
	physicsWorld->addEntity(havokObjects[havokObjects.getSize()-1]);
	convexShape->removeReference();
}

void CHavokInit::DeleteAllObjects() 
{
	std::cout<<havokObjects.getSize()<<" havok objects deleted after a successful \"DeleteAllObjects\" call.\n";
	for(int i=0;i<havokObjects.getSize();i++) {
		physicsWorld->removeEntity(havokObjects[i]);
	}
	havokObjects.clear();
	allObjects.clear();
}

void CHavokInit::Initialize()
{
	hkMemorySystem::FrameInfo finfo(4096 * 4096);	// Allocate memory to Physics solver buffer. This amount of memory is capable of storing a large amount of objects!
	memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, finfo);
	hkBaseSystem::init( memoryRouter, errorReport );
	hkpWorldCinfo worldInfo;
	worldInfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_2ITERS_HARD);
	worldInfo.m_gravity = hkVector4(0.0f, -9.8f, 0.0f);
	worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY; // just fix the entity if the object falls off too far
	worldInfo.setBroadPhaseWorldSize(1000.0f); // objects are not be simulated outside this region
	physicsWorld = new hkpWorld(worldInfo);
	hkpAgentRegisterUtil::registerAllAgents( physicsWorld->getCollisionDispatcher());
	CreateGround();	
}

void CHavokInit::SetGravity(float _x, float _y, float _z) 
{
	hkVector4 temp =  hkVector4(_x*10,_y*10,_z*10);
	physicsWorld->setGravity(temp);
	for(int i=0;i<havokObjects.getSize();i++)
	{
		havokObjects[i]->activate();//to activate sleeping objects
	}
}

void CHavokInit::RenderAllObjects() 
{
	for(int i=0;i<allObjects.size();i++)
	{
		allObjects[i]->DrawSelf();
	}
}

void CHavokInit::SetProperMatrix(int i)
{
				float qw = havokObjects[i]->getRotation()(3);
				float qx = -havokObjects[i]->getRotation()(0); //with these negative the problem that was referenced in the thesis is solved
				float qy = -havokObjects[i]->getRotation()(1); //it was solved after the printing of the thesis...
				float qz = -havokObjects[i]->getRotation()(2);
				float n = 1.0f/sqrt(qx*qx+qy*qy+qz*qz+qw*qw);
				qx *= n;
				qy *= n;
				qz *= n;
				qw *= n;
				//http://stackoverflow.com/questions/1556260/convert-quaternion-rotation-to-rotation-matrix
				allObjects[i]->myMatrix[0]=1.0f - 2.0f*qy*qy - 2.0f*qz*qz;//this conversion was done using the explanation in the link from above
				allObjects[i]->myMatrix[1]=2.0f*qx*qy - 2.0f*qz*qw;
				allObjects[i]->myMatrix[2]=2.0f*qx*qz + 2.0f*qy*qw;
				allObjects[i]->myMatrix[3]=0;
				allObjects[i]->myMatrix[4]=2.0f*qx*qy + 2.0f*qz*qw;
				allObjects[i]->myMatrix[5]=1.0f - 2.0f*qx*qx - 2.0f*qz*qz;
				allObjects[i]->myMatrix[6]=2.0f*qy*qz - 2.0f*qx*qw;
				allObjects[i]->myMatrix[7]=0;
				allObjects[i]->myMatrix[8]=2.0f*qx*qz - 2.0f*qy*qw;
				allObjects[i]->myMatrix[9]=2.0f*qy*qz + 2.0f*qx*qw;
				allObjects[i]->myMatrix[10]=1.0f - 2.0f*qx*qx - 2.0f*qy*qy;
				allObjects[i]->myMatrix[11]=0;
				allObjects[i]->myMatrix[12]=havokObjects[i]->getPosition()(0);
				allObjects[i]->myMatrix[13]=havokObjects[i]->getPosition()(1);
				allObjects[i]->myMatrix[14]=havokObjects[i]->getPosition()(2);;
				allObjects[i]->myMatrix[15]=1;
}

void CHavokInit::StepSim(float time_step) 
{
	physicsWorld->stepDeltaTime(1.0f/(time_step*2));
	for (int i = 0; i < allObjects.size(); i++ ) 
	{
		if(havokObjects[i]->isActive())SetProperMatrix(i);
	} 	
}

void CHavokInit::StopSim() 
{

}

int CHavokInit::GetNumberOfObjects()
{
	return allObjects.size();
}
