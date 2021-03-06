#include "Common.h"
#include <stdio.h>
#include <future>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "ControllerView.h"
#include "Values.h"


ControllerView::ControllerView()
{
	m_pCurrentWorldContext = NULL;

	m_worldWidth = DEFAULT_WORLD_WIDTH;
	m_worldHeight = DEFAULT_WORLD_HEIGHT;
	m_worldAlgorithm = DEFAULT_ALGORITHM;
	m_density = DEFAULT_CELLS_DENSITY;
	m_prm1 = 1;
	m_prm2 = 0;
	m_prm3 = 0;
	m_prm4 = 0;

	m_drawInterval = DEFAULT_DRAW_INTERVAL;
	m_skipNum = DEFAULT_SKIP_NUM;

	initLibrary();
	initUI();
}

ControllerView::~ControllerView()
{
	TwTerminate();
	glutDestroyWindow(m_windowId);
}

ControllerView* ControllerView::getInstance()
{
	static ControllerView s_controllerView;
	return &s_controllerView;
}

void ControllerView::setCurrentWorldContext(WorldContext* context)
{
	m_pCurrentWorldContext = context;
}

void ControllerView::onUpdate(void)
{
	TwSetCurrentWindow(glutGetWindow());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	TwDraw();
	glFlush();
	glutPostRedisplay();
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void ControllerView::onResize(int w, int h)
{
	glViewport(0, 0, w, h);
	TwSetCurrentWindow(glutGetWindow());
	TwWindowSize(w, h);
	//char str[10];
	//sprintf_s(str, 10, "%d %d", width, height);
	//TwSetParam(m_pBar, NULL, "size", TW_PARAM_CSTRING, 1, str);
}

int ControllerView::MouseButtonCB(int glutButton, int glutState, int mouseX, int mouseY)
{
	TwSetCurrentWindow(glutGetWindow());
	return TwEventMouseButtonGLUT(glutButton, glutState, mouseX, mouseY);
}

int ControllerView::MouseMotionCB(int mouseX, int mouseY)
{
	TwSetCurrentWindow(glutGetWindow());
	return TwEventMouseMotionGLUT(mouseX, mouseY);
}


int ControllerView::KeyboardCB(unsigned char glutKey, int mouseX, int mouseY)
{
	TwSetCurrentWindow(glutGetWindow());
	if (glutKey != 'g') {
		if (getInstance()->m_pCurrentWorldContext && getInstance()->m_pCurrentWorldContext->m_pView)
			getInstance()->m_pCurrentWorldContext->m_pView->onKeyboard(glutKey, mouseX, mouseY);
	} else {
		new WorldContext();
	}
	return TwEventKeyboardGLUT(glutKey, mouseX, mouseY);
}

int ControllerView::SpecialKeyCB(int glutKey, int mouseX, int mouseY)
{
	TwSetCurrentWindow(glutGetWindow());
	return TwEventSpecialGLUT(glutKey, mouseX, mouseY);
}


void ControllerView::initLibrary()
{
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGB);
	glClearColor(0, 0, 0, 1);
	m_windowId = glutCreateWindow("Controller");
	glutCreateMenu(NULL);
	glutDisplayFunc(ControllerView::onUpdate);
	glutReshapeFunc(ControllerView::onResize);
	static int s_init = 0;
	if (s_init == 0) {
		TwInit(TW_OPENGL, NULL);
		s_init = 1;
	}
	
	glutMouseFunc((GLUTmousebuttonfun)MouseButtonCB);
	glutMotionFunc((GLUTmousemotionfun)MouseMotionCB);
	glutPassiveMotionFunc((GLUTmousemotionfun)MouseMotionCB);
	glutKeyboardFunc((GLUTkeyboardfun)KeyboardCB);
	glutSpecialFunc((GLUTspecialfun)SpecialKeyCB);
	TwGLUTModifiersFunc(glutGetModifiers);
}


void TW_CALL ControllerView::onClickBtnStartStop(void * clientData)
{
	ControllerView* pControllerView = (ControllerView*)clientData;
	if(pControllerView->m_pCurrentWorldContext && getInstance()->m_pCurrentWorldContext->m_pView)
		pControllerView->m_pCurrentWorldContext->m_pView->onKeyboard('p', 0, 0);
}

void TW_CALL ControllerView::onClickBtnStep(void * clientData)
{
	ControllerView* pControllerView = (ControllerView*)clientData;
	if (pControllerView->m_pCurrentWorldContext && getInstance()->m_pCurrentWorldContext->m_pView)
		pControllerView->m_pCurrentWorldContext->m_pView->onKeyboard('1', 0, 0);
}

void TW_CALL ControllerView::onClickBtnAlloc(void * clientData)
{
	ControllerView* pControllerView = (ControllerView*)clientData;
	if (pControllerView->m_pCurrentWorldContext && getInstance()->m_pCurrentWorldContext->m_pView)
		pControllerView->m_pCurrentWorldContext->m_pView->onKeyboard('a', 0, 0);
}

void TW_CALL ControllerView::onClickBtnClear(void * clientData)
{
	ControllerView* pControllerView = (ControllerView*)clientData;
	if (pControllerView->m_pCurrentWorldContext && getInstance()->m_pCurrentWorldContext->m_pView)
		pControllerView->m_pCurrentWorldContext->m_pView->onKeyboard('c', 0, 0);
}

void TW_CALL ControllerView::onClickBtnAllocAll(void * clientData)
{
	ControllerView* pControllerView = (ControllerView*)clientData;
	if (pControllerView->m_pCurrentWorldContext && getInstance()->m_pCurrentWorldContext->m_pView)
		pControllerView->m_pCurrentWorldContext->m_pView->onKeyboard('A', 0, 0);
}

void TW_CALL ControllerView::onClickBtnClearAll(void * clientData)
{
	ControllerView* pControllerView = (ControllerView*)clientData;
	if (pControllerView->m_pCurrentWorldContext && getInstance()->m_pCurrentWorldContext->m_pView)
		pControllerView->m_pCurrentWorldContext->m_pView->onKeyboard('C', 0, 0);
}

void TW_CALL ControllerView::onClickBtnInformation(void * clientData)
{
	ControllerView* pControllerView = (ControllerView*)clientData;
	if (pControllerView->m_pCurrentWorldContext && getInstance()->m_pCurrentWorldContext->m_pView)
		pControllerView->m_pCurrentWorldContext->m_pView->onKeyboard('i', 0, 0);
}

void TW_CALL ControllerView::onClickBtnWorldGenerate(void * clientData)
{
	new WorldContext();
}

void TW_CALL ControllerView::onClickBtnWorldUpdate(void * clientData)
{
	if (getInstance()->m_pCurrentWorldContext) {
		delete getInstance()->m_pCurrentWorldContext;
		getInstance()->m_pCurrentWorldContext = NULL;
	}
	new WorldContext();
}

void TW_CALL ControllerView::onClickBtnWorldLoad(void * clientData)
{
	WorldContext *context = WorldContext::generateFromFile();
}

void TW_CALL ControllerView::onClickBtnWorldSave(void * clientData)
{
	if (getInstance()->m_pCurrentWorldContext) {
		getInstance()->m_pCurrentWorldContext->m_pLogic->stopRun();
		WorldContext::saveToFile(getInstance()->m_pCurrentWorldContext);
	}
}

void TW_CALL ControllerView::onClickBtnWorldQuit(void * clientData)
{
	if (getInstance()->m_pCurrentWorldContext) {
		delete getInstance()->m_pCurrentWorldContext;
		getInstance()->m_pCurrentWorldContext = NULL;
	}
}

void ControllerView::initUI()
{
	char str[64];
	TwSetCurrentWindow(glutGetWindow());
	m_pBar = TwNewBar("Controller");

	TwDefine(" Controller position = '0 0' movable=false resizable=false ");
	int colWidth = WINDOW_WIDTH / 2;
	TwSetParam(m_pBar, NULL, "valueswidth", TW_PARAM_INT32, 1, &colWidth);
	sprintf_s(str, "%d %d", WINDOW_WIDTH, WINDOW_HEIGHT);
	TwSetParam(m_pBar, NULL, "size", TW_PARAM_CSTRING, 1, str);
	TwDefine(" GLOBAL fontsize=3 fontstyle=fixed ");

	TwAddVarRW(m_pBar, "width", TW_TYPE_INT32, &m_worldWidth, " min=32 max=16384 step=32 label='Width' group='World Parameters' ");
	TwAddVarRW(m_pBar, "height", TW_TYPE_INT32, &m_worldHeight, " min=32 max=16384 step=32 label='Height' group='World Parameters' ");
	TwEnumVal algorithmEV[] = { 
		{ ALGORITHM_NORMAL, "Normal" },
		{ ALGORITHM_NORMAL_MP, "Normal Open MP" },
		{ ALGORITHM_NORMAL_NON_TORUS, "Normal(Non Torus)" },
		{ ALGORITHM_NORMAL_NON_TORUS_MP, "Normal(Non Torus) Open MP" },
		//{ ALGORITHM_NORMAL_NON_TORUS_CUDA, "Normal(Non Torus) CUDA" },
		{ ALGORITHM_GROUP_MP, "Group Open MP" },
#ifdef ENABLE_CUDA
		{ ALGORITHM_NORMAL_CUDA, "Normal CUDA" },
		{ ALGORITHM_GROUP_CUDA, "Group CUDA" },
#endif
	};
	TwType algorithmType = TwDefineEnum("AlgorithmType", algorithmEV, sizeof(algorithmEV)/sizeof(TwEnumVal));
	TwAddVarRW(m_pBar, "Algorithm", algorithmType, &m_worldAlgorithm, " keyIncr='<' keyDecr='>' help='Change algorithm.' group='World Parameters' ");
	TwAddButton(m_pBar, "btnWorldUpdate", ControllerView::onClickBtnWorldUpdate, this, " label='Re-generate ' group='World Parameters' ");
	TwAddButton(m_pBar, "btnWorldGenerate", ControllerView::onClickBtnWorldGenerate, this, " label='Generate new world[g]' group='World Parameters' ");
	TwAddButton(m_pBar, "btnWorldLoad", ControllerView::onClickBtnWorldLoad, this, " label='Load World' group='World Parameters' ");
	TwAddButton(m_pBar, "btnWorldSave", ControllerView::onClickBtnWorldSave, this, " label='Save World' group='World Parameters' ");
	TwAddButton(m_pBar, "btnWorldQuit", ControllerView::onClickBtnWorldQuit, this, " label='Quit [q]' group='World Parameters' ");
	
	TwAddSeparator(m_pBar, NULL, NULL);
	TwAddButton(m_pBar, "btnInformation", ControllerView::onClickBtnInformation, this, " label='Show Information [i]' group='View' ");
	TwAddVarRW(m_pBar, "drawInterval", TW_TYPE_INT32, &m_drawInterval, "min=1 max=10 step=1 label='Draw interval' group='View' ");
	TwAddVarRW(m_pBar, "skipNum", TW_TYPE_INT32, &m_skipNum, "min=1 max=100 step=1 label='Skip' group='View' ");

	TwAddSeparator(m_pBar, NULL, NULL);
	TwAddButton(m_pBar, "textLClick", NULL, this, " label='Left click/drag to put/clear cells' group='Operations' ");
	TwAddButton(m_pBar, "textRClick", NULL, this, " label='Right drag to move area' group='Operations' ");
	TwAddButton(m_pBar, "textWheel", NULL, this, " label='Wheel to zoom in/out' group='Operations' ");
	TwAddButton(m_pBar, "textCClick", NULL, this, " label='Center click to initialize view' group='Operations' ");
	TwAddButton(m_pBar, "textPattern", NULL, this, " label='[l] to load pattern, [s] to save pattern' group='Operations' ");
	TwAddButton(m_pBar, "btnStartStop", ControllerView::onClickBtnStartStop, this, " label='Play [p]' group='Operations' ");
	TwAddButton(m_pBar, "btnStep", ControllerView::onClickBtnStep, this, " label='Step [1]' group='Operations' ");
	TwAddButton(m_pBar, "btnAlloc", ControllerView::onClickBtnAlloc, this, " label='Alloc [a]' group='Operations' ");
	TwAddButton(m_pBar, "btnClear", ControllerView::onClickBtnClear, this, " label='Clear [c]' group='Operations' ");
	TwAddButton(m_pBar, "btnAllocAll", ControllerView::onClickBtnAllocAll, this, " label='Alloc all [A]' group='Operations' ");
	TwAddButton(m_pBar, "btnClearAll", ControllerView::onClickBtnClearAll, this, " label='Clear all [C]' group='Operations' ");
	TwAddVarRW(m_pBar, "density", TW_TYPE_INT32, &m_density, "min=0 max=100 step=1 label='Density' group='Operations' ");
	TwAddVarRW(m_pBar, "prm1", TW_TYPE_INT32, &m_prm1, "min=0 max=1 step=1 label='Group' group='Operations' ");
	TwAddVarRW(m_pBar, "prm2", TW_TYPE_INT32, &m_prm2, "min=0 max=100 step=1 label='prm2' group='Operations' ");
	TwAddVarRW(m_pBar, "prm3", TW_TYPE_INT32, &m_prm3, "min=0 max=100 step=1 label='prm3' group='Operations' ");
	TwAddVarRW(m_pBar, "prm4", TW_TYPE_INT32, &m_prm4, "min=0 max=100 step=1 label='prm4' group='Operations' ");
}

