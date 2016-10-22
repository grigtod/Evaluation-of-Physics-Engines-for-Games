#include <iostream>
#include <vector>
#include "PhysXInit.h"
#include "BulletInit.h"
#include "HavokInit.h"
#include <GL/freeglut.h>
#include <AntTweakBar.h>
#include <fstream> //recordig data

//method declarations
void ShutDown();
void InitGL();
void OnRender();
void DrawGrid(int);
void OnIdle();
void OnReshape(int, int);
void Mouse(int, int, int, int);
void MouseWheel(int, int, int, int);
void Motion(int, int);
void Mouse(int button, int s, int x, int y);
void Keyboard (unsigned char, int, int);
void DefineAllEngines();
void CalculateFPS();
void GravitySetting();
//gui
void CreateMainBar() ;
void CreateOtherBar();
void CreatePerformanceBar();
void TW_CALL BoxesSpawn(void *clientData);
void TW_CALL SphereSpawn(void *clientData);
void TW_CALL CapsuleSpawn(void *clientData);
void TW_CALL ConvexSpawn(void *clientData);
void TW_CALL ClearButtonFunc(void *clientData);
void TW_CALL ClearLog(void *clientData);

//variable declarations
int winWidth = 1024;
int winHeight = 600;
float zoom = 20.0f;
int		oldX = 0;
int		oldY = 0;
float	rX = 15;
float	rY = 0;
float	dist = 0;
double  timeLastFrame = 0;
double	timeNow = 0;
double	frame = 0;
double	fpsValue = 0;
double	msValue = 0;
bool	renderActorsBool = true;
int		simulationTimeStep = 60;
//more GUI declarations
////main bar
TwBar *mainBar;  
typedef enum { PHYSX, BULLET, HAVOK} EEngines;
EEngines engineEnum = PHYSX;
typedef enum { DEFAULT, CONCRETE, RUBBER } MType;
CObjectBase::materialType matEnum = CObjectBase::CONCRETE;
TwEnumVal enginesEV[] = { {PHYSX, "PhysX"}, {BULLET, "Bullet"}, {HAVOK, "Havok"} };
TwType listOfEngines; //was called "allEngines" before!!!
TwEnumVal matsEV[] = { {DEFAULT, "Default"}, {CONCRETE, "Concrete"}, {RUBBER, "Rubber"}};
TwType allMats;
int numberOfActors;
int spawnX = 5;
int spawnY = 5;
int spawnZ = 5;
int totalToBeSpawned=0;
//performance tab 
TwBar *perfBar;  
//other tab 
TwBar *otherBar;  
typedef enum {ZEROG ,SUN, JUPITER, SATURN, URANUS, NEPTUNE, EARTH, VENUS, MARS, MOON, CUSTOM } EGravities;
EGravities gravityEnum = EARTH;
TwEnumVal presetGravsEV[] = { {ZEROG, "zeroG"},{SUN, "Sun"}, {JUPITER, "Jupiter"}, {SATURN, "Saturn"}, {URANUS, "Uranus"}, {NEPTUNE, "Neptune"}, {EARTH, "Earth"}, {VENUS, "Venus"}, {MARS, "Mars"}, {MOON, "Moon"}, {CUSTOM, "Custom"}  };
TwType allGravities;
float gravityDirection[3] = {0.00f, -9.78f, 0.00f};
float oldGravityDirection[3] = {0.00f, 0.00f, 0.00f};
float simSpeed;
//engine declarations
std::vector <CPhysicsBase*> allEngines;
//convex shapes
GLMmodel * bunnyOBJ = glmReadOBJ("../Assets/bunny.obj");
//http://www.thingiverse.com/thing:151081/#files source of the bunny
//file management
std::ofstream fileController;
char fileName[] = "perfMeasurements.txt";
bool measurePerf = false;
int	perfFreq = 10;
int framesPassed = 0;

void TW_CALL ClearLog(void *clientData) //to clear the data text file
{
	fileController.open("PerformanceLog.txt");
	fileController<<"Engine | Frame | Objects count | fps | ms"<<std::endl<<std::endl;
	fileController.close();
}

void main(int argc, char** argv)
{
	//hide the console. To unhide it just comment 
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0); 
	//hide the console end
	
	atexit(ShutDown); // The "ShutDown" function is called when the user exits the application
	
	glutInit(&argc, argv); // Initialize the glut (freeglut) library
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight); //screen and height
	glutCreateWindow("Physics Engine Evaluation"); //creates a new glut window

	TwInit(TW_OPENGL, NULL); // Initialize the antTweakBar 
	glutDisplayFunc(OnRender); // the display fuction is OnRender
	glutIdleFunc(OnIdle); //the idle function is OnIdle
	glutReshapeFunc(OnReshape);
	glutMouseFunc(Mouse);
	glutMouseWheelFunc(MouseWheel);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	//create gui bars...
	CreateMainBar();
	CreateOtherBar();
	CreatePerformanceBar();
	InitGL();//call the freeglut initialization
	DefineAllEngines();
	glutMainLoop();		
}

void DefineAllEngines()
{
	//all engines will be added ti the allEngine vector here.
	//this makes it possible to extend the list of engines in the future, without the need to edit the main...
	allEngines.push_back(new CPhysXInit ());
	allEngines.push_back(new CBulletInit ());
	allEngines.push_back(new CHavokInit ());
}

void CalculateFPS()
{
	frame++;
	double timeNow = glutGet(GLUT_ELAPSED_TIME);
	if(timeNow - timeLastFrame >=1)
	{
		msValue = timeNow-timeLastFrame;
		fpsValue = (1000/msValue);
		//std::cout<<msValue<<"ms"<<std::endl;
		//std::cout<<fpsValue<<"fps"<<std::endl<<std::endl;
		timeLastFrame = timeNow;
		frame = 0;
	}
}

void OnRender() 
{
	//saving in performance log file
	if(measurePerf)
	{
		framesPassed++;
		if(framesPassed%perfFreq==0)
		{//Current Engine | Current Frame | Objects count | Performance fps | performance ms"<<std::endl<<std::endl;
		fileController.open("PerformanceLog.txt",std::ios_base::app);
		fileController<< engineEnum <<" | "<< framesPassed <<" | "<< numberOfActors <<" | "<< fpsValue <<"fps | "<<msValue <<"ms"<<std::endl;

		fileController.close();
		}
	}
	else
	{
		framesPassed=0;
	}
	//end saving performance log in file
	GravitySetting();//set the gravity 
	totalToBeSpawned = spawnX * spawnY * spawnZ;//calculate a feedback totalToBeSpawned int
	CalculateFPS();//calculate the fps of the simulation
	numberOfActors = allEngines[(int)engineEnum]->GetNumberOfObjects();
	if(totalToBeSpawned>500)//Additional warning if someone wants to spawn many objects at once...
	{
		TwDefine("SpawnAndClean/Total label='Total !!!'  "); 
	}
	else 
	{
		TwDefine("SpawnAndClean/Total label='Total' "); 
	}
	if(numberOfActors>1000)
	{
		TwDefine("SpawnAndClean/Spawned label='Spawned !!!' "); 
	}
	else
	{
		TwDefine("SpawnAndClean/Spawned label='Spawned' "); 
	}
	GLdouble viewer[3]= {zoom*sin(0.0),zoom,zoom*cos(0.0)};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0,0,dist);
	gluLookAt(viewer[0],viewer[1],viewer[2], 0, 0, 0, 0, 1, 0);
	glRotatef(rX,1,0,0);
	glRotatef(rY,0,1,0);
	DrawGrid(10);//draw the grid
	glEnable(GL_LIGHTING);	
	glColor3f(0.5,0.5,0.5);
	allEngines[(int)engineEnum]->StepSim(simulationTimeStep);//step the simulation of the current engine
	if(renderActorsBool)allEngines[(int)engineEnum]->RenderAllObjects(); //render all the actors of the current engine
	glDisable(GL_LIGHTING);
	TwDraw();//draw the gui
	glutSwapBuffers();
}

void InitGL() { 
	glEnable(GL_DEPTH_TEST);//standard freeglut initialization...
	glEnable(GL_CULL_FACE); 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat ambient[4]={0.25f,0.25f,0.25f,0.25f};
	GLfloat diffuse[4]={1,1,1,1};
	GLfloat mat_diffuse[4]={0.5f,0.5,0.5,0.5};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glDisable(GL_LIGHTING);
}

void DrawGrid(int _gridSize) //standard glut grid...
{ 
	glBegin(GL_LINES);
	glColor3f(0.8f, 0.8f, 0.8f);
	for(int i=-_gridSize;i<=_gridSize;i++) {
		glVertex3f((float)i,0,(float)-_gridSize);
		glVertex3f((float)i,0,(float)_gridSize);
		glVertex3f((float)-_gridSize,0,(float)i);
		glVertex3f((float)_gridSize,0,(float)i);
	}
	glEnd();
}

void OnIdle() 
{
	glutPostRedisplay();
}

void OnReshape(int nw, int nh) //standard reshape function
{
	glViewport(0,0,nw, nh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100, (GLfloat)nw / (GLfloat)nh, 0.1f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	TwWindowSize(nw, nh);
}

void Mouse(int button, int s, int x, int y) //standard ouse move function
{
	if(!TwEventMouseButtonGLUT(button, s, x, y))
	{
		if (s == GLUT_DOWN) {
			oldX = x; 
			oldY = y; 
		}
	}
}

void MouseWheel(int button, int dir, int x, int y) {

    if (dir > 0) {
       if(zoom>4) zoom-=1.0f;
    }
    else {
       if(zoom<100) zoom+=1.0f;
    }
}

void Motion(int x, int y) 
{
	if( !TwEventMouseMotionGLUT(x, y) )  // send event to AntTweakBar
		{ 
			rY += (x - oldX)/5.0f; 
			rX += (y - oldY)/5.0f; 
			oldX = x; 
			oldY = y; 
		glutPostRedisplay(); 
	}
}

void Keyboard (unsigned char key, int x, int y) 
{
	if(!TwEventKeyboardGLUT(key, x, y)) {
		switch (key) {
		case 27: exit (0);// Escape key glutDestroyWindow ( Win.id ); 
		break;
		default:
		break;
		}
	}
}

void ShutDown() 
{
	
}

void GravitySetting() 
{
		switch(gravityEnum)
		{
		case ZEROG:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = 0.0f;
			gravityDirection[2] = 0.0f;
		break;
		case SUN:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -274.0f;
			gravityDirection[2] = 0.0f;
		break;
		case JUPITER: 
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -24.79f;
			gravityDirection[2] = 0.0f;
		break;
		case SATURN:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -10.44f;
			gravityDirection[2] = 0.0f;
		break;
		case URANUS:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -8.69f;
			gravityDirection[2] = 0.0f;
		break;
		case NEPTUNE:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -11.15f;
			gravityDirection[2] = 0.0f;
		break;
		case EARTH:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -9.78;
			gravityDirection[2] = 0.0f;
		break;
		case VENUS:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -8.87f;
			gravityDirection[2] = 0.0f;
		break;
		case MARS:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -3.77f;
			gravityDirection[2] = 0.0f;
		break;
		case MOON:
			TwDefine("OtherSettings/GravityDirection visible='false' ");
			gravityDirection[0] = 0.0f;
			gravityDirection[1] = -1.62f;
			gravityDirection[2] = 0.0f;
		break;
		case CUSTOM: TwDefine("OtherSettings/GravityDirection visible='true' "); 
		break;
		
	}
	//sets the gravity for all the engines!!!

	if((oldGravityDirection[0] != gravityDirection[0])
	|| (oldGravityDirection[1] != gravityDirection[1])
	|| (oldGravityDirection[2] != gravityDirection[2]))
	{
		for(int i=0;i<allEngines.size();i++)
		{
			allEngines[i]->SetGravity( gravityDirection[0], gravityDirection[1], gravityDirection[2] );	
		}

		oldGravityDirection[0] = gravityDirection[0];
		oldGravityDirection[1] = gravityDirection[1];
		oldGravityDirection[2] = gravityDirection[2];
	}
}

void CreateMainBar() 
{
	mainBar = TwNewBar("SpawnAndClean"); 
	TwDefine("SpawnAndClean size='190 300' color='0 120 0' position='10 10'  label='Settings' "); 
	TwDefine("SpawnAndClean resizable=false ");
	
	listOfEngines = TwDefineEnum("SpawnAndClean", enginesEV, 3);
	TwAddVarRW(mainBar, "Engine:", listOfEngines, &engineEnum, NULL);
	TwAddVarRW(mainBar, "Timestep", TW_TYPE_INT32, &simulationTimeStep, "min=1 max=500 label= 'Timestep: 1/' help='The timestep of the application' ");
	TwAddSeparator(mainBar,NULL,NULL);
	allMats = TwDefineEnum("Material", matsEV, 3);
	TwAddVarRW(mainBar, "Material:", allMats, &matEnum, NULL);
	TwAddVarRW(mainBar, "Xspawn", TW_TYPE_INT32, &spawnX, " label= 'X' min=1 max=100 step=1 help='Spawn on X' ");
	TwAddVarRW(mainBar, "Yspawn", TW_TYPE_INT32, &spawnY, " label= 'Y' min=1 max=100 step=1 help='Spawn on Y' ");
	TwAddVarRW(mainBar, "Zspawn", TW_TYPE_INT32, &spawnZ, " label= 'Z' min=1 max=100 step=1 help='Spawn on Z' ");
	//TwAddSeparator(mainBar,NULL,NULL);
	TwAddVarRW(mainBar, "Total", TW_TYPE_INT32, &totalToBeSpawned, " label= 'Total' min=1 max=1000000 step=1 help='Total to be spawned'  ");
	TwAddSeparator(mainBar,NULL,NULL);
	TwAddButton(mainBar, "BoxesSpawning", BoxesSpawn, NULL, " label='Spawn boxes' key=b help='Spawn boxes' ");
	TwAddButton(mainBar, "SphereSpawning", SphereSpawn, NULL, " label='Spawn spheres' key=s help='Spawn spheres' ");
	TwAddButton(mainBar, "CapsuleSpawning", CapsuleSpawn, NULL, " label='Spawn capsules' key=c help='Spawn capsules' ");
	TwAddButton(mainBar, "ConvexSpawning", ConvexSpawn, NULL, " label='Spawn convex' key=m help='Spawn convex' ");
	TwAddSeparator(mainBar,NULL,NULL);
	TwAddVarRW(mainBar, "Spawned", TW_TYPE_INT32, &numberOfActors, " label= 'Spawned'  help='Spawned' ");
	TwAddSeparator(mainBar,NULL,NULL);
	TwAddButton(mainBar, "Clean", ClearButtonFunc, NULL, " label='Clean' help='Clean all the actors' ");
}

void CreateOtherBar() 
{
	otherBar = TwNewBar("OtherSettings"); 
	TwDefine("OtherSettings size='190 200' color='0 120 0' position='10 360'  label='World Settings' "); 
	TwDefine("OtherSettings resizable=false ");
	allGravities = TwDefineEnum("OtherSettings", presetGravsEV, 11);
	TwAddVarRW(otherBar, "Gravity:", allGravities, &gravityEnum, NULL);
	TwAddSeparator(mainBar,NULL,NULL);
	TwAddVarRW(otherBar, "GravityDirection", TW_TYPE_DIR3F, &gravityDirection," label='Gravity direction' open help='Change the gravity direction.' ");
}

void CreatePerformanceBar() 
{
	perfBar = TwNewBar("PerformanceBar"); 
	TwDefine("PerformanceBar refresh=0.07 ");
	TwDefine("PerformanceBar size='250 160' color='0 120 0' position='550 10'  label='Rendering:' "); 
	TwAddVarRW(perfBar, "RenderActors", TW_TYPE_BOOLCPP, &renderActorsBool, "help='Do you want to render the objects.' label='Render objects:' "); 
	TwAddSeparator(perfBar,NULL,NULL);
	TwAddVarRW(perfBar, "FPS", TW_TYPE_DOUBLE, &fpsValue, " label= 'fps' help='The frames per second of the application' ");
	TwAddVarRW(perfBar, "MS", TW_TYPE_DOUBLE, &msValue, " label= 'ms' help='How many miliseconds it took for the last frame' ");
	TwAddSeparator(perfBar,NULL,NULL);
	TwAddVarRW(perfBar, "Measure performance", TW_TYPE_BOOLCPP, &measurePerf, " label='Measure performance.' "); 
	TwAddVarRW(perfBar, "Measure frequency", TW_TYPE_INT32, &perfFreq, " label= 'Measure frequency' help='in fps' ");
	TwAddButton(perfBar, "CleanLog", ClearLog, NULL, " label='Clean Log' key=b help='Clean Log' ");
}

void TW_CALL BoxesSpawn(void *clientData) 
{
	for( int i = 0; i < spawnY; i++ ) {
		for( int j = 0; j < spawnX; j++ ) {
			for( int q = 0; q < spawnZ; q++ ) {
				allEngines[(int)engineEnum]->CreateBox(-spawnX/2 + j*1, 10 + i*1, -spawnZ/2 + q*1,1,1,1,matEnum);
			}
		}
	}
}

void TW_CALL SphereSpawn(void *clientData) 
{
	for( int i = 0; i < spawnY; i++ ) {
		for( int j = 0; j < spawnX; j++ ) {
			for( int q = 0; q < spawnZ; q++ ) {
				allEngines[(int)engineEnum]->CreateSphere(-spawnX/2 + j*1, 10 + i*1, -spawnZ/2 + q*1,1,matEnum);
			}
		}
	}                       
}

void TW_CALL CapsuleSpawn(void *clientData) 
{
	for( int i = 0; i < spawnY; i++ ) {
		for( int j = 0; j < spawnX; j++ ) {
			for( int q = 0; q < spawnZ; q++ ) {
				allEngines[(int)engineEnum]->CreateCapsule(-spawnX/2 + j*1, 10 + i*1, -spawnZ/2 + q*1, 0.5, 1.5, matEnum); 
			}
		}
	}
}

void TW_CALL ConvexSpawn(void *clientData) 
{
	for( int i = 0; i < spawnY; i++ ) {
		for( int j = 0; j < spawnX; j++ ) {
			for( int q = 0; q < spawnZ; q++ ) {
					allEngines[(int)engineEnum]->CreateConvex(*bunnyOBJ, -spawnX/2 + j*1, 10 + i*1, -spawnZ/2 + q*1,1,1,1,matEnum);
			}
		}
	}
}

void TW_CALL ClearButtonFunc(void *clientData) 
{
	allEngines[(int)engineEnum]->DeleteAllObjects();
}