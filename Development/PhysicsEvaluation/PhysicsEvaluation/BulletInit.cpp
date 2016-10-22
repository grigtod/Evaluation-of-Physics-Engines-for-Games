#include "BulletInit.h"

CBulletInit::CBulletInit(void)
{
	Initialize();
}

CBulletInit::~CBulletInit(void)
{
	//release/delete all pointers here!!!!!!!!!!
	DeleteAllObjects();
	delete dynamicsWorld;
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
}

float CBulletInit::ReturnMassForMaterial(CObjectBase::materialType _matType)
{
	switch(_matType) //set the mass according to the material
	{
	case CObjectBase::CONCRETE: 
		return 2.0f; 
	break;
	case CObjectBase::RUBBER: 
		return 0.2f; 
	break;
	case CObjectBase::DEFFAULT: 
		return 1.0f; 
	break;
	}
}

void CBulletInit::AdjustConstructionInfo(btRigidBody::btRigidBodyConstructionInfo &tempRigidBodyCI, CObjectBase &tempObject, CObjectBase::materialType _matType)
{
	switch(_matType) //additional material adjustments
	{	
	case CObjectBase::CONCRETE: 
		tempObject.myMaterial = CObjectBase::CONCRETE;
		tempRigidBodyCI.m_restitution = 0.09f;
		tempRigidBodyCI.m_linearDamping = 0.02f;
		tempRigidBodyCI.m_friction = 100.0f;
	break;
	case CObjectBase::RUBBER: 
		tempObject.myMaterial = CObjectBase::RUBBER;
		tempRigidBodyCI.m_restitution = 1.05f;
		tempRigidBodyCI.m_linearDamping = 0.01f;
		tempRigidBodyCI.m_friction = 50.0f;
	break;
	case CObjectBase::DEFFAULT: 
		tempObject.myMaterial = CObjectBase::DEFFAULT;
		tempRigidBodyCI.m_restitution = 1.05f;
		tempRigidBodyCI.m_linearDamping = 0.01f;
		tempRigidBodyCI.m_friction = 50.0f;
	break;
	}
}

void CBulletInit::CreateGround() 
{
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0))),new btStaticPlaneShape(btVector3(0,1,0),1),btVector3(0,0,0));
	groundRigidBodyCI.m_restitution = 0.0;
	groundRigidBodyCI.m_friction = 20.0;
	//groundRigidBodyCI.m_mass = 1.0f;
	dynamicsWorld->addRigidBody(new btRigidBody(groundRigidBodyCI));
}

void CBulletInit::CreateBox(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ) 
{
	CBulletInit::CreateBox(_spawnPosX,_spawnPosY,_spawnPosZ,_sizeX,_sizeY,_sizeZ, CObjectBase::DEFFAULT);
}

void CBulletInit::CreateBox(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ, CObjectBase::materialType _matType) 
{
	btCollisionShape* tempShape = new btBoxShape(btVector3(_sizeX/2,_sizeY/2,_sizeZ/2));
	btScalar tempMass = ReturnMassForMaterial(_matType); //adjust the material mass
	allObjects.push_back(new CBox());
	btVector3 tempInertia(0,0,0);
    tempShape->calculateLocalInertia(tempMass,tempInertia);
	btTransform transform = btTransform(btQuaternion(0,0,0,1),btVector3(_spawnPosX, _spawnPosY, _spawnPosZ)); 
    btRigidBody::btRigidBodyConstructionInfo tempRigidBodyCI(tempMass,new btDefaultMotionState(transform),tempShape,tempInertia); //create a construction info 
	AdjustConstructionInfo(tempRigidBodyCI,*allObjects[allObjects.size()-1],_matType);//adjust the construction info
	bulletObjects.push_back(new btRigidBody(tempRigidBodyCI)); 
	dynamicsWorld->addRigidBody(bulletObjects[bulletObjects.size()-1]); //instead of using additional temp pointer to hold the rigidody...
}

void CBulletInit::CreateSphere(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _radius) 
{
	CBulletInit::CreateSphere(_spawnPosX,_spawnPosY,_spawnPosZ,_radius, CObjectBase::DEFFAULT);
}

void CBulletInit::CreateSphere(float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _radius, CBox::materialType _matType) 
{
	btCollisionShape* tempShape = new btSphereShape(_radius/2);
	btScalar tempMass;
	tempMass = ReturnMassForMaterial(_matType); //adjust the material mass
	allObjects.push_back(new CSphere());
    btVector3 tempInertia(0,0,0);
    tempShape->calculateLocalInertia(tempMass,tempInertia);
	btTransform transform = btTransform(btQuaternion(0,0,0,1),btVector3(_spawnPosX, _spawnPosY, _spawnPosZ)); 
    btRigidBody::btRigidBodyConstructionInfo tempRigidBodyCI(tempMass,new btDefaultMotionState(transform),tempShape,tempInertia);
	AdjustConstructionInfo(tempRigidBodyCI,*allObjects[allObjects.size()-1],_matType);
	bulletObjects.push_back(new btRigidBody(tempRigidBodyCI)); 
	dynamicsWorld->addRigidBody(bulletObjects[bulletObjects.size()-1]); //instead of using additional temp pointer to hold the rigidody...
	
}

void CBulletInit::CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height) {
	CBulletInit::CreateCapsule(start_pos_x, start_pos_y,start_pos_z,radius,height, CObjectBase::DEFFAULT);
}

void CBulletInit::CreateCapsule(float start_pos_x, float start_pos_y, float start_pos_z, float radius, float height, CObjectBase::materialType _matType) 
{
	btCollisionShape* tempShape = new btCapsuleShape(radius, height);
	btTransform transform = btTransform(btQuaternion(0,0,0,1),btVector3(start_pos_x, start_pos_y, start_pos_z)); 
	btScalar tempMass;
	tempMass = ReturnMassForMaterial(_matType); //adjust the material mass
	allObjects.push_back(new CCapsule());
	allObjects[allObjects.size()-1]->SetSize(radius,height,radius); //adjust the capsule size
	allObjects[allObjects.size()-1]->SetDrawingRot(90,0,0); //adjust the rendering rotation offset
    btVector3 tempInertia(0,0,0); //no initial inertia
    tempShape->calculateLocalInertia(tempMass,tempInertia); 
    btRigidBody::btRigidBodyConstructionInfo tempRigidBodyCI(tempMass,new btDefaultMotionState(transform),tempShape,tempInertia); //create a new construction info object
	AdjustConstructionInfo(tempRigidBodyCI,*allObjects[allObjects.size()-1],_matType); //adjust the construction info and the temp object
	bulletObjects.push_back(new btRigidBody(tempRigidBodyCI)); 
	dynamicsWorld->addRigidBody(bulletObjects[bulletObjects.size()-1]); //instead of using additional temp pointer to hold the rigidody...
}

void CBulletInit::CreateConvex(GLMmodel &objmodel_ptr, float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ) 
{
	CBulletInit::CreateConvex(objmodel_ptr, _spawnPosX,_spawnPosY,_spawnPosZ,_sizeX,_sizeY,_sizeZ, CObjectBase::DEFFAULT);
}

void CBulletInit::CreateConvex(GLMmodel &objmodel_ptr, float _spawnPosX,float _spawnPosY,float _spawnPosZ,float _sizeX,float _sizeY,float _sizeZ, CObjectBase::materialType _matType) 
{
	btConvexHullShape* tempShape = new btConvexHullShape();
	for(int i=3;i<(objmodel_ptr.numvertices*3+3);i+=3)//because of the way the obj holds its data
	{
		tempShape->addPoint(btVector3(objmodel_ptr.vertices[i],objmodel_ptr.vertices[i+1],objmodel_ptr.vertices[i+2]));
	}
	btTransform transform = btTransform(btQuaternion(0,0,0,1),btVector3(_spawnPosX, _spawnPosY, _spawnPosZ)); 
	btScalar tempMass = ReturnMassForMaterial(_matType); //adjust the material mass
	allObjects.push_back(new CConvex(objmodel_ptr));
    btVector3 tempInertia(0,0,0);
    tempShape->calculateLocalInertia(tempMass,tempInertia);
    btRigidBody::btRigidBodyConstructionInfo tempRigidBodyCI(tempMass,new btDefaultMotionState(transform),tempShape,tempInertia);
	AdjustConstructionInfo(tempRigidBodyCI,*allObjects[allObjects.size()-1],_matType);
	bulletObjects.push_back(new btRigidBody(tempRigidBodyCI)); 
	dynamicsWorld->addRigidBody(bulletObjects[bulletObjects.size()-1]); //instead of using additional temp pointer to hold the rigidody...
}

void CBulletInit::DeleteAllObjects()
{
	std::cout<<bulletObjects.size()<<" bullet objects deleted after a successful \"DeleteAllObjects\" call.\n";
	for(int i=0;i<bulletObjects.size();i++)
	{
		dynamicsWorld->removeRigidBody(bulletObjects[i]);
	}
	bulletObjects.clear();
	allObjects.clear();
}

void CBulletInit::Initialize() 
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-9.81,0));
	CreateGround();
}

void CBulletInit::SetGravity(float _x, float _y, float _z) 
{
	btVector3 temp =  btVector3(_x,_y,_z); 
	dynamicsWorld->setGravity(temp);
	for(int i=0;i<bulletObjects.size();i++)
	{
		bulletObjects[i]->activate();
	}
}

void CBulletInit::StepSim(float _timeStep) 
{
	dynamicsWorld->stepSimulation(1.0f/(_timeStep),1);
	for (int i = 0; i < allObjects.size(); i++ ) {
		bulletObjects[i]->getWorldTransform().getOpenGLMatrix(allObjects[i]->myMatrix);//conversion of the matrix to opengl is very easy
	} 
}

void CBulletInit::RenderAllObjects() {
	for(int i=0;i<allObjects.size();i++)
	{
		allObjects[i]->DrawSelf();
	}
}

void CBulletInit::StopSim() {

}

int CBulletInit::GetNumberOfObjects()
{
	return allObjects.size();
}

