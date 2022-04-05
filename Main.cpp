#include <Windowsx.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#define WINDOW_TITLE "GgBot"

int qNo = 1;


// transformation
float tX, tY, tZ = 0;
float tSpeed = 0.5;

// set Perspective View as default
boolean isOrtho = false;
float orthoNear = -20, orthoFar = 20.0;
float perspecNear = 10, perspecFar = 20;

// mouse movements
float mouseLastX = 0.0f, mouseLastY = 0.0f;
float mouseXRotate = 0.0f, mouseYRotate = 0.0f, mouseZRotate = 0.0f;
float mouseZoomLevel = -2.0f;

// other (Seperate to your own sections too)


LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		switch (wParam) {
		case MK_LBUTTON:
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			mouseYRotate -= xPos - mouseLastX;
			mouseXRotate -= yPos - mouseLastY;
			mouseLastX = xPos;
			mouseLastY = yPos;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		mouseLastX = GET_X_LPARAM(lParam);
		mouseLastY = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONUP:
		//if (!textureOn)
			//textureOn = true;
		break;
	case WM_MOUSEWHEEL:
		mouseZoomLevel -= GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == 0x31) {	// press key '1'
			qNo = 1;
		}
		else if (wParam == 0x32) {
			qNo = 2;
		}
		else if (wParam == 0x33) {
			qNo = 3;
		}
		else if (wParam == VK_UP) {
			if (isOrtho) {
				if (tZ > orthoNear)
					tZ += tSpeed;
			}
			//else {
			//	if (tZ > perspecNear) 
			//		tZ -= tSpeed;
			//}
		}
		else if (wParam == VK_DOWN) {
			if (isOrtho) {
				if (tZ < orthoFar)
					tZ -= tSpeed;
			}
			//else {
			//	if (tZ < perspecFar) 
			//		tZ += tSpeed;
			//}
		}
		else if (wParam == VK_LEFT) {
			tX -= tSpeed;
		}
		else if (wParam == VK_RIGHT) {
			tX += tSpeed;
		}
		else if (wParam == VK_SPACE) {
			tX = 0, tY = 0, tZ = 0;
			mouseXRotate = 0.0f, mouseYRotate = 0.0f, mouseZRotate = 0.0f;
			mouseZoomLevel = -2.0f;

		}
		else if (wParam == 'O') {
			isOrtho = true;
			tZ = 0.0;
		}
		else if (wParam == 'P') {
			isOrtho = false;
			tZ = 0.0;
		}
		else if (wParam == 'S') {
			if (tSpeed > 0) {
				tSpeed = 0.0;
			}
			else {
				tSpeed = 0.5;
			}
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void projection() {
	glMatrixMode(GL_PROJECTION);		// refer to Projection Matrix
	glLoadIdentity();					// reset the projection matrix

	if (isOrtho) {
		glOrtho(-10, 10, -10, 10, orthoNear, orthoFar);	// Ortho default is 1.0
	}
	else {
		gluPerspective(35, 1, -1, 1);
		glFrustum(-10, 10, -10, 10, perspecNear, perspecFar);
		glEnable(GL_DEPTH_TEST);
	}
}

void renderSphere(float r) {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_LINE);
	gluSphere(sphere, r, 50, 50);
	gluDeleteQuadric(sphere);
}

void renderFillCylinder(float baseR, float topR, float h) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluCylinder(obj, baseR, topR, h, 50, 50);
	gluDeleteQuadric(obj);
}

void renderLineCylinder(float baseR, float topR, float h) {
	glLineWidth(1);
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);
	gluCylinder(obj, baseR, topR, h, 20, 10);
	gluDeleteQuadric(obj);
}

void renderDisk(float inR, float outR, float slices, float loops) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	//gluQuadricDrawStyle(obj, GLU_LINE);	// GLU_FILL, GLU_SILHOUETTE, GLU_POINT
	gluQuadricDrawStyle(obj, GLU_LINE);	// GLU_FILL, GLU_SILHOUETTE, GLU_POINT
	//gluQuadricDrawStyle(obj, GLU_LINE);	// GLU_FILL, GLU_SILHOUETTE, GLU_POINT
	//gluQuadricDrawStyle(obj, GLU_LINE);	// GLU_FILL, GLU_SILHOUETTE, GLU_POINT
	gluDisk(obj, inR, outR, slices, loops);
	gluDeleteQuadric(obj);
}

void renderPartialDisk(float inR, float outR, float slices, float loops, float startAng, float endAngle) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);
	gluPartialDisk(obj, inR, outR, slices, loops, startAng, endAngle);
	gluDeleteQuadric(obj);
}

void renderSphereWithoutGLU()
{
	const float PI = 3.141592f;
	GLfloat x, y, z, sliceA, stackA;
	GLfloat radius = 0.5;
	int sliceNo = 30, stackNo = 30;
	for (sliceA = 0.0; sliceA < PI; sliceA += PI / sliceNo)
	{
		glBegin(GL_LINE_STRIP);
		for (stackA = 0.0; stackA < PI; stackA += PI / stackNo)
		{
			x = radius * cos(stackA) * sin(sliceA);
			y = radius * sin(stackA) * sin(sliceA);
			z = radius * cos(sliceA);
			glColor3f(0.3, 0.8, 0.5);
			glVertex3f(x, y, z);
			x = radius * cos(stackA) * sin(sliceA + PI / stackNo);
			y = radius * sin(stackA) * sin(sliceA + PI / sliceNo);
			z = radius * cos(sliceA + PI / sliceNo);
			glColor3f(0.5, 0.3, 0.8);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void renderCube(float x, float y, float z) {
	glBegin(GL_LINE_LOOP);

	glVertex3f(-x, -y, -z);
	glVertex3f(-x, -y, z);
	glVertex3f(x, -y, z);
	glVertex3f(x, -y, -z);

	glVertex3f(-x, -y, -z);
	glVertex3f(-x, y, -z);
	glVertex3f(x, y, -z);
	glVertex3f(x, -y, -z);

	glVertex3f(-x, -y, -z);
	glVertex3f(-x, -y, z);
	glVertex3f(-x, y, z);
	glVertex3f(-x, y, -z);

	glVertex3f(x, y, -z);
	glVertex3f(x, y, z);
	glVertex3f(-x, y, z);
	glVertex3f(-x, y, -z);

	glVertex3f(x, y, -z);
	glVertex3f(x, y, z);
	glVertex3f(x, -y, z);
	glVertex3f(x, -y, -z);

	glVertex3f(x, -y, z);
	glVertex3f(x, y, z);
	glVertex3f(-x, y, z);
	glVertex3f(-x, -y, z);

	glEnd();
}

void renderTrapezium(float top, float bot1, float bot2, float y, float z) {
	glBegin(GL_LINE_LOOP);

	glVertex3f(bot1, 0.0f, 0.0f);
	glVertex3f(bot1, 0.0f, z);
	glVertex3f(bot2, 0.0f, z);
	glVertex3f(bot2, 0.0f, 0.0f);

	glVertex3f(bot1, 0.0f, 0.0f);
	glVertex3f(0.0f, y, 0.0f);
	glVertex3f(top, y, 0.0f);
	glVertex3f(bot2, 0.0f, 0.0f);

	glVertex3f(bot1, 0.0f, 0.0f);
	glVertex3f(bot1, 0.0f, z);
	glVertex3f(0.0f, y, z);
	glVertex3f(0.0f, y, 0.0f);

	glVertex3f(top, y, 0.0f);
	glVertex3f(top, y, z);
	glVertex3f(0.0f, y, z);
	glVertex3f(0.0f, y, 0.0f);

	glVertex3f(top, y, 0.0f);
	glVertex3f(top, y, z);
	glVertex3f(bot2, 0.0f, z);
	glVertex3f(bot2, 0.0f, 0.0f);

	glVertex3f(bot2, 0.0f, z);
	glVertex3f(top, y, z);
	glVertex3f(0.0f, y, z);
	glVertex3f(bot1, 0.0f, z);

	glEnd();
}

void renderCuboid(float x, float y, float z) {

	glBegin(GL_LINE_LOOP);
	glColor3f(0.5, 0.5, 0);

	//	Face 1 - Bottom
	glVertex3f(0.0, y, 0.0);
	glVertex3f(x, y, 0.0);
	glVertex3f(x, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	// Face 2 - Left
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, z);
	glVertex3f(0.0, y, z);
	glVertex3f(0.0, y, 0.0);
	// Face 3 - Top
	glVertex3f(0.0, y, 0.0);
	glVertex3f(x, y, 0.0);
	glVertex3f(x, y, z);
	glVertex3f(0.0, y, z);
	// Face 4 - Front	
	glVertex3f(0.0, y, z);
	glVertex3f(0.0, 0.0, z);
	glVertex3f(x, 0.0, z);
	glVertex3f(x, y, z);
	// Face 5 - Right  
	glVertex3f(x, y, z);
	glVertex3f(x, 0.0, z);
	glVertex3f(x, 0.0, 0.0);
	glVertex3f(x, y, 0.0);
	// Face 6 - Back 
	glVertex3f(x, y, 0.0);
	glVertex3f(x, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, y, 0.0);

	glEnd();
}


void drawHead() {}

void drawBody() {}

void drawLeftArm() {}

void drawRightArm() {}

void drawLeftLeg() {

}

void drawRightLeg() {}

void drawSpine() {
	glColor3f(1.0, 0.0, 0.0);
	renderCube(3,3,3);
}

void drawGgBot() {
	glPushMatrix();
	{

		drawHead();

		drawBody();

		drawLeftArm();

		drawRightArm();

		drawLeftLeg();

		drawRightLeg();

	}
	glPopMatrix();
}

void robotStructure() {
	glPushMatrix();
	glTranslatef(0, 2, 0);	// move to centre on window
	{
		//glScalef(0.10, 0.10, 0.10);

		// head
		glPushMatrix();
		glTranslatef(0, 3.5, 0);
		renderCube(0.75, 0.75, 0.75);
		glPopMatrix();

		// body 
		glPushMatrix();
		renderCube(1, 2, 0.75);
		glPopMatrix();

		// left hand
		glPushMatrix();
		glTranslatef(-1.85, -0.5, 0);
		renderCube(0.5, 2.5, 0.5);
		glPopMatrix();

		// right hand
		glPushMatrix();
		glTranslatef(1.85, -0.5, 0);
		renderCube(0.5, 2.5, 0.5);
		glPopMatrix();

		// left leg
		glPushMatrix();
		glTranslatef(-0.65, -5.5, 0);
		renderCube(0.5, 3.5, 0.75);
		glPopMatrix();

		// right leg
		glPushMatrix();
		glTranslatef(0.65, -5.5, 0);
		renderCube(0.5, 3.5, 0.75);
		glPopMatrix();

		// left hand connection point
		glPushMatrix();
		glTranslatef(-1.2, 1.5, 0);
		renderSphere(0.25);
		glPopMatrix();

		// right hand connection point
		glPushMatrix();
		glTranslatef(1.2, 1.5, 0);
		renderSphere(0.25);
		glPopMatrix();

	}
	glPopMatrix();
}


void test1() {
	glPushMatrix();
	{
		glColor3f(0.0, 1.0, 1.0);
		//glTranslatef(tX, tY, tZ);
		glRotatef(tX, 0, 1, 0);	// test rotation, look cool mah
		tX += tSpeed;			// press s to stop rotate, press s again to start rotate

		glPushMatrix();
		{
			glTranslatef(0, 5.5, 0);	// move sphere up to head
			renderSphere(1.5);
		}
		glPopMatrix();

		robotStructure();

		glColor3f(1.0, 1.0, 0.0);
		drawGgBot();

	}
	glPopMatrix();
}

void test2() {
	glColor3f(1.0, 1.0, 0.0);
	drawGgBot();

	renderDisk(0.0f, 1.0f, 5, 10);	// pentagon

	glTranslatef(0, 2, 0);
	renderDisk(0.2f, 1.0f, 20, 10);	// disk

	glTranslatef(0, 2, 0);
	renderDisk(0.7f, 1.0f, 20, 10);	// donut disk

	glTranslatef(0, 2, 0);
	renderPartialDisk(0.0f, 1.0f, 20, 10, 0, 180);	// half disk
}

void test3() {
	glColor3f(1.0, 0.0, 1.0);
	drawLeftArm();
	renderLineCylinder(1, 1, 7);
	glPushMatrix();

	glPopMatrix();
}

void display()
{
	// project initialization
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		glEnable(GL_DEPTH_TEST);

		projection();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//*****mouse movement start***** 
		glTranslatef(0.0, 0.0, mouseZoomLevel);
		glRotatef(mouseXRotate, 1.0, 0.0, 0.0);
		glRotatef(mouseYRotate, 0.0, 1.0, 0.0);
		glRotatef(mouseZRotate, 0.0, 0.0, 1.0);
		//*****mouse movement end*****
	}

	switch (qNo) {
	case 1:
		test1();	// main robot structure here for ref
		break;
	case 2:
		//test2();	// change to what u want for test
		break;
	case 3:
		test3();	// change to what u want for test
		break;
	default:
		test1();
		break;
	}

}



bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	int n = ChoosePixelFormat(hdc, &pfd);

	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000,
		NULL, NULL, wc.hInstance, NULL);

	HDC hdc = GetDC(hWnd);

	initPixelFormat(hdc);

	HGLRC hglrc = wglCreateContext(hdc);

	if (!wglMakeCurrent(hdc, hglrc)) return false;

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}