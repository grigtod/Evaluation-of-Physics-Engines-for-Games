#include "PhysXInit.h"

using namespace physx;
CPhysXInit::CPhysXInit(void) {
	myPhysicsSDK = NULL; 
	myFoundation = NULL;
	myDefaultFilterShader=PxDefaultSimulationFilterShader;
	myScene = NULL;
	Initialize();
}

CPhysXInit::~CPhysXInit(void)  {
	DeleteAllObjects(); //get rid of all the pointers
	myCpuDispatcher->release();
	myPhysicsSDK->release();
	myScene->release();
	myFoundation->release();
	myGroundPlane->release(); 
	myGroundShape->release();
}

void CPhysXInit::CreateGround() {//cretes a new static object with a set transform
	myGroundPlane = myPhysicsSDK->createRigidStatic(PxTransform(PxVec3(0.0f, 0, 0.0f),PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f))));
	//cretes a new ground plane with a set material. The longer way to do this is to create the material pointer first...but than I'll have to release it
	myGroundShape = myGroundPlane->createShape(PxPlaneGeometry(), *myPhysicsSDK->createMaterial(0.5,0.5,0.5));//i've experimented with the frictions and the restitution and this one seems to work well 
	myScene->addActor(*myGroundPlane);//add the plane actor to the scene. This could be made even shorter, but it will be very hard to read and understand...
}

float CPhysXInit::GetDensity(CObjectBase::materialType _matType)
{
	switch(_matType) {//returns the density according to the material
	case CObjectBase::CONCRETE:					
			return 4;
		break;
	case CObjectBase::RUBBER:
			return 0.3;
		break;
	default:
			return 1;
		break;
	}	
}

void CPhysXInit::CreateBox(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ) {
	
	CPhysXInit::CreateBox(_spawnPosX,_spawnPosY,_spawnPosZ,_sizeX,_sizeY,_sizeZ, CObjectBase::DEFFAULT);
}

void CPhysXInit::CreateBox(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ, CObjectBase::materialType _matType) {
	
	PxVec3 posVec = PxVec3(_spawnPosX,_spawnPosY,_spawnPosZ);
	PxTransform transform(posVec, PxQuat::createIdentity());
	PxVec3 dimensions(0.5f*_sizeX,0.5f*_sizeY,0.5f*_sizeZ);
	PxBoxGeometry geometry(dimensions);
	PxMaterial* tempMaterial = NULL; 
	allObjects.push_back(new CBox());
	MaterialSetter(_matType, tempMaterial,*allObjects.back());
	physXObjects.push_back(PxCreateDynamic(*myPhysicsSDK, transform, geometry, *tempMaterial, GetDensity(_matType)));
	ObjectProperties(_matType, physXObjects.size()-1);
	myScene->addActor(*physXObjects.back());
	allObjects.back()->SetSize(0.5f*_sizeX,0.5f*_sizeY,0.5f*_sizeZ);
	tempMaterial->release();
}

void CPhysXInit::CreateSphere(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _radius) {
	
	CPhysXInit::CreateSphere(_spawnPosX,_spawnPosY,_spawnPosZ,_radius, CObjectBase::DEFFAULT);
}

void CPhysXInit::CreateSphere(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _radius, CBox::materialType _matType) {
	
	PxVec3 posVec = PxVec3(_spawnPosX,_spawnPosY,_spawnPosZ);
	PxTransform transform(posVec, PxQuat::createIdentity());
	PxVec3 dimensions(0.4);//_radius;
	PxBoxGeometry geometry(dimensions);
	PxMaterial* tempMaterial = NULL; 
	allObjects.push_back(new CSphere());
	MaterialSetter(_matType, tempMaterial,*allObjects.back());
	physXObjects.push_back(PxCreateDynamic(*myPhysicsSDK, transform, geometry, *tempMaterial, GetDensity(_matType)));
	ObjectProperties(_matType, physXObjects.size()-1);
	myScene->addActor(*physXObjects.back());
	allObjects.back()->SetSize(_radius,_radius,_radius);
	tempMaterial->release();
}

void CPhysXInit::CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height) {
	CPhysXInit::CreateCapsule(start_pos_x, start_pos_y,start_pos_z,radius,height, CObjectBase::DEFFAULT);
}

void CPhysXInit::CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height, CObjectBase::materialType _matType) {
	
	PxVec3 posVec = PxVec3(start_pos_x,start_pos_y,start_pos_z);
	PxTransform transform(posVec, PxQuat::createIdentity());
	PxCapsuleGeometry  geometry(radius, height);
	PxMaterial* tempMaterial = NULL; 
	allObjects.push_back(new CCapsule());
	MaterialSetter(_matType, tempMaterial,*allObjects.back());
	physXObjects.push_back(PxCreateDynamic(*myPhysicsSDK, transform, geometry, *tempMaterial, GetDensity(_matType)));
	ObjectProperties(_matType, physXObjects.size()-1);
	myScene->addActor(*physXObjects.back());
	allObjects.back()->SetSize(radius,height,radius);
	allObjects.back()->SetDrawingRot(0,90,0);
	tempMaterial->release();
}

void CPhysXInit::CreateConvex(GLMmodel &objmodel_ptr, float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ) {
	
	CPhysXInit::CreateConvex(objmodel_ptr, _spawnPosX,_spawnPosY,_spawnPosZ,_sizeX,_sizeY,_sizeZ, CObjectBase::DEFFAULT);
}

void CPhysXInit::CreateConvex(GLMmodel &objmodel_ptr, float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ, CObjectBase::materialType _matType) {
	std::vector <PxVec3> meshVerts; 
	PxConvexMesh * myConvexMesh;
	static const PxVec3 convexVerts[1] ;//seen in the documentation of physX...usually used for cooking of convexes
	PxConvexMeshDesc convexDesc; //it took a lot of time to get it to work !
	convexDesc.points.count     = objmodel_ptr.numvertices; //might not be the best way to do it...
	convexDesc.points.stride    = sizeof(PxVec3);
	convexDesc.points.data      = objmodel_ptr.vertices;
	convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX;
	convexDesc.vertexLimit      = 256;
	PxCooking *cooking = PxCreateCooking(PX_PHYSICS_VERSION, myPhysicsSDK->getFoundation(), PxCookingParams(PxTolerancesScale()));
	PxDefaultMemoryOutputStream buf;
    if(cooking->cookConvexMesh(convexDesc, buf))
	{
		PxDefaultMemoryInputData id(buf.getData(), buf.getSize());
		myConvexMesh = myPhysicsSDK->createConvexMesh(id);
	}
	PxVec3 posVec = PxVec3(_spawnPosX,_spawnPosY,_spawnPosZ);
	PxTransform transform(posVec, PxQuat::createIdentity());
	PxBoxGeometry geometry(PxVec3(0.5f*_sizeX,0.5f*_sizeY,0.5f*_sizeZ));
	PxMaterial* tempMaterial = NULL; 
	allObjects.push_back(new CConvex(objmodel_ptr));
	MaterialSetter(_matType, tempMaterial,*allObjects.back());
	physXObjects.push_back(PxCreateDynamic(*myPhysicsSDK,transform,PxConvexMeshGeometry(myConvexMesh),*tempMaterial,GetDensity(_matType)));
	ObjectProperties(_matType, physXObjects.size()-1);
	myScene->addActor(*physXObjects.back());
	allObjects.back()->SetSize(0.5f*_sizeX,0.5f*_sizeY,0.5f*_sizeZ);
	tempMaterial->release();
}

void CPhysXInit::MaterialSetter(CObjectBase::materialType _matType, PxMaterial *& _tempMat, CObjectBase   & tempBoxHelper) 
{
	switch(_matType) {
	case CObjectBase::CONCRETE:						//static friction, dynamic friction, restitution
			_tempMat = myPhysicsSDK->createMaterial(0.1,0.1,0.1); 
			tempBoxHelper.myMaterial = CObjectBase::CONCRETE;
		break;
	case CObjectBase::RUBBER:
			_tempMat = myPhysicsSDK->createMaterial(0.5,0.5,0.5); 
			tempBoxHelper.myMaterial = CObjectBase::RUBBER;
		break;
	default:
			_tempMat = myPhysicsSDK->createMaterial(0.5,0.5,0.5); 
			tempBoxHelper.myMaterial = CObjectBase::DEFFAULT;
		break;
	}
}

void CPhysXInit::ObjectProperties(CObjectBase::materialType _matType, int _i) 
{
	switch(_matType) {
	case CObjectBase::CONCRETE:						
			physXObjects[_i]->setMass(5.0f);
			//physXObjects.back()->setLinearVelocity(PxVec3(0,0,0)); 
		break;
	case CObjectBase::RUBBER:
			physXObjects[_i]->setMass(0.01f);
			//physXObjects.back()->setLinearVelocity(PxVec3(0,0,0)); 
		break;
	default:
			physXObjects[_i]->setMass(2.0f);
			//physXObjects.back()->setLinearVelocity(PxVec3(0,0,0)); 	
		break;
	}
}

void CPhysXInit::DeleteAllObjects() 
{
	std::cout<<physXObjects.size()<<" physX objects deleted after a successful \"DeleteAllObjects\" call.\n";

	for(int i=0;i<physXObjects.size();i++) {
		myScene->removeActor(*physXObjects[i]);
	}
	physXObjects.clear();
	allObjects.clear();
}

void CPhysXInit::Initialize() 
{
	///https://devtalk.nvidia.com/default/topic/540296/unhandled-exception-when-creating-physics/?offset=8
	//Initialization see in the link  up
	myFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, myDefaultAllocatorCallback, myDefaultErrorCallback); 
	myPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *myFoundation, PxTolerancesScale()); 
	//CreateScene
	PxSceneDesc tempSceneDesc(myPhysicsSDK->getTolerancesScale());
	tempSceneDesc.gravity=PxVec3(0.0f, -90.81f, 0.0f);//for some reasong gravity works well when multiplied by 10...
	myCpuDispatcher = PxDefaultCpuDispatcherCreate(1); 
	tempSceneDesc.cpuDispatcher = myCpuDispatcher;
    tempSceneDesc.filterShader  = myDefaultFilterShader;
	myScene = myPhysicsSDK->createScene(tempSceneDesc);
	CreateGround();
}

void CPhysXInit::SetGravity(float _x, float _y, float _z) 
{
	PxVec3 tempGravity =  PxVec3(_x*10,_y*10,_z*10);
	myScene->setGravity(tempGravity);
	for(int i=0;i<physXObjects.size();i++)
	{
		physXObjects[i]->addForce(PxVec3(0.00f,0.00f,0.00f)); // a hack to wake up all the objects...
	}
}

void CPhysXInit::StepSim(float _timeStep) 
{
	myScene->simulate(1.0f/(_timeStep*2)); //for some reason it's broken        
	while(!myScene->fetchResults() ) {
		for (int i = 0; i < physXObjects.size(); i++ ) {
			SetMatrix(PxMat33(physXObjects[i]->getGlobalPose().q), PxVec3 (physXObjects[i]->getGlobalPose().p), allObjects[i]->myMatrix);
		} 
	}
}

void CPhysXInit::SetMatrix(PxMat33 m, PxVec3 t, float *mat) {
	mat[0] = m.column0[0];
	mat[1] = m.column0[1];
	mat[2] = m.column0[2];
	mat[3] = 0;

	mat[4] = m.column1[0];
	mat[5] = m.column1[1];
	mat[6] = m.column1[2];
	mat[7] = 0;

	mat[8] = m.column2[0];
	mat[9] = m.column2[1];
	mat[10] = m.column2[2];
	mat[11] = 0;

	mat[12] = t[0];
	mat[13] = t[1];
	mat[14] = t[2];
	mat[15] = 1;
}

void CPhysXInit::RenderAllObjects() {
	for(int i=0;i<allObjects.size();i++)
	{
		allObjects[i]->DrawSelf();
	}
}

void CPhysXInit::StopSim() {

}

int CPhysXInit::GetNumberOfObjects()
{
	return allObjects.size();
}