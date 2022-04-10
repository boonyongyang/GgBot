#include <Windowsx.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#define WINDOW_TITLE "GgBot"

int qNo = 3;
float stacks = 30;

// transformation for projection matrix
float ptX, ptY, prY = 0;
float ptSpeed = 0.1;

// transformation for object
float tX, tY, tZ = 0;
float tSpeed = 0.5;

// set Perspective View as default
boolean isOrtho = false;
float orthoNear = -20, orthoFar = 20.0;
float perspecNear = 10, perspecFar = 20;

// mouse movements
float mouseLastX = 0.0f, mouseLastY = 0.0f;
float mouseXRotate = 0.0f, mouseYRotate = 0.0f, mouseZRotate = 0.0f;
float perspecZoomLevel = -2.0f;

// other (Seperate to your own sections too)
// head animation
float hx = 0, hy = 0, hz = 0, hAngle = 0, hSpeed = 1;

//texture
BITMAP BMP;				// bitmap structure
HBITMAP hBMP = NULL;	// bitmap handle

// Lighting
float lax = 0, lay = -1, laz = 0;
float ldx = 0, ldy = 1, ldz = 0;
float amb[] = { 1.0, 1.0, 1.0 ,1.0};			// red color ambient light 
// light0 position (0,6,0) above 
float posA[] = { lax, lay, laz };			// position of the light0 {x,y,z} - position 0,0,0 is actually inside the sphere 
float dif[] ={ 0.0, 0.0, 1.0 ,1.0 };		// green color diffuse light
float posD[] = { ldx, ldy, ldz };		// position of the light1 {x,y,z} - position 0,0,0 is actually inside the sphere

float ambM[] = { 1.0, 1.0, 1.0 ,1.0 };		// red color ambient material
float difM[] = { 1.0, 1.0, 1.0 ,1.0 };		// blue color diffuse material

boolean isLightOn = true;


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
		perspecZoomLevel -= GET_WHEEL_DELTA_WPARAM(wParam) / 110.0f;
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
		else if (wParam == VK_SPACE) {
			tX = 0, tY = 0, tZ = 0;
			mouseXRotate = 0.0f, mouseYRotate = 0.0f, mouseZRotate = 0.0f, perspecZoomLevel = -2.0f;
			ptX = 0, ptY = 0, prY = 0, hAngle = 0;

		}
		else if (wParam == VK_UP) {
			if (isOrtho) {
				if (tZ > orthoNear)
					tZ += tSpeed;	// still need to change, to make robot won't dissapear 
			}
			else {
				perspecZoomLevel += 1;	// temporary solution, need to change to tZ and make robot won't dissapear 

			}
		}
		else if (wParam == VK_DOWN) {
			if (isOrtho) {
				if (tZ < orthoFar)
					tZ -= tSpeed;	// still need to change, to make robot won't dissapear 
			}
			else {
				perspecZoomLevel -= 1;	// temporary solution, need to change to tZ and make robot won't dissapear 
			}
		}
		else if (wParam == VK_LEFT) {
			if (tX > -10)
				tX -= tSpeed;

		}
		else if (wParam == VK_RIGHT) {
			if (tX < 10)
				tX += tSpeed;
		}
		else if (wParam == 'A') {
			if (isOrtho) {
				if (ptX < 1.1)
					ptX += ptSpeed;
			}
			else {
				prY -= ptSpeed * 15;	// perspective rotate y-axis, object look flat
			}
		}
		else if (wParam == 'D') {
			if (isOrtho) {
				if (ptX > -1.1)
					ptX -= ptSpeed;
			}
			else {
				prY += ptSpeed * 15;	// perspective rotate y-axis, object look flat
			}
		}
		else if (wParam == 'W') {
			if (isOrtho) {
				if (ptY > -1.3)
					ptY -= ptSpeed;
			}
			else {
				perspecZoomLevel -= 1;
			}
		}
		else if (wParam == 'S') {
			if (isOrtho) {
				if (ptY < 1.3)
					ptY += ptSpeed;
			}
			else {
				perspecZoomLevel += 1;
			}
		}
		else if (wParam == 'O') {
			isOrtho = true;
			tZ = 0.0;
		}
		else if (wParam == 'P') {
			isOrtho = false;
			tZ = 0.0;
		}
		else if (wParam == 'F') {
			if (tSpeed > 0) {
				tSpeed = 0.0;
			}
			else {
				tSpeed = 0.5;
			}
		}
		else if (wParam == 'L') {
			isLightOn = !isLightOn;
		}
		else if (wParam == 'M') {
			hAngle += hSpeed;
		}
		else if (wParam == 'N') {
			hAngle -= hSpeed;
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

	glTranslatef(ptX, ptY, 0.0f);		// projection translation
	glRotatef(prY, 0.0, 1.0, 0.0);		// projection rotation (y-axis only)

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
	gluQuadricDrawStyle(sphere, GLU_LINE);	// will change to GLU_FILL
	gluSphere(sphere, r, 50, 50);
	gluDeleteQuadric(sphere);
}

void renderDisk(float inR, float outR, float slices, float loops) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL);	// will change to GLU_FILL		// GLU_LINE, GLU_SILHOUETTE, GLU_POINT
	gluQuadricTexture(obj, true);
	gluDisk(obj, inR, outR, slices, loops);
	gluDeleteQuadric(obj);
}

void renderPartialDisk(float inR, float outR, float slices, float loops, float startAng, float endAngle) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);	// will change to GLU_FILL
	gluPartialDisk(obj, inR, outR, slices, loops, startAng, endAngle);
	gluDeleteQuadric(obj);
}

void renderPolygon(float baseR, float topR, float h, float slices) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL);	// will change to GLU_FILL
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluQuadricTexture(obj, true);
	gluCylinder(obj, baseR, topR, h, slices, 50);
	gluDeleteQuadric(obj);
}

void renderCylinder(float baseR, float topR, float h) {
	int slices = 50;
	renderPolygon(baseR, topR, h, slices);
	renderDisk(0, baseR, slices, stacks); 

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, h);
	renderDisk(0, topR, slices, stacks);
	glPopMatrix();
}

void renderCone(float topR, float h) {
	int slices = 50;
	renderPolygon(0, topR, h, slices);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, h);
	renderDisk(0, topR, slices, stacks);	// cone only need one disk
	glPopMatrix();
}

void renderPyramid(float topR, float h) {
	int slices = 3;
	renderPolygon(0, topR, h, slices);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, h);
	renderDisk(0, topR, slices, stacks);	// pyramid only need one disk
	glPopMatrix();
}

void renderPrism(float l, float h, float slices) {
	renderPolygon(l, l, h, slices);
	renderDisk(0, l, slices, stacks);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, h);
	renderDisk(0, l, slices, stacks);
	glPopMatrix();
}

void renderCuboid(float l, float h) {
	int slices = 4;
	glPushMatrix();
	{
		glRotatef(45, 0, 0, 1);
		renderPolygon(l, l, h, slices);
		renderDisk(0, l, slices, stacks);

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, h);
		renderDisk(0, l, slices, stacks);
		glPopMatrix();
	}
	glPopMatrix();
}

void renderTrapezoid(float baseL, float topL, float h) {
	int slices = 4;
	glPushMatrix();
	{
		glRotatef(45, 0, 0, 1);
		renderPolygon(baseL, topL, h, slices);
		renderDisk(0, baseL, slices, stacks);

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, h);
		renderDisk(0, topL, slices, stacks);
		glPopMatrix();
	}
	glPopMatrix();
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

void renderCubeWithoutGLU(float x, float y, float z) {

	glPushMatrix();
	{
		//glColor3f(1.0, 1.0, 1.0);

		glBegin(GL_QUADS);

		glTexCoord2f(0, 1);
		glVertex3f(-x, -y, -z);
		glTexCoord2f(1, 1);
		glVertex3f(-x, -y, z);
		glTexCoord2f(1, 0);
		glVertex3f(x, -y, z);
		glTexCoord2f(0, 0);
		glVertex3f(x, -y, -z);

		glTexCoord2f(0, 1);
		glVertex3f(-x, -y, -z);
		glTexCoord2f(1, 1);
		glVertex3f(-x, y, -z);
		glTexCoord2f(1, 0);
		glVertex3f(x, y, -z);
		glTexCoord2f(0, 0);
		glVertex3f(x, -y, -z);

		glTexCoord2f(0, 1);
		glVertex3f(-x, -y, -z);
		glTexCoord2f(1, 1);
		glVertex3f(-x, -y, z);
		glTexCoord2f(1, 0);
		glVertex3f(-x, y, z);
		glTexCoord2f(0, 0);
		glVertex3f(-x, y, -z);

		glTexCoord2f(0, 1);
		glVertex3f(x, y, -z);
		glTexCoord2f(1, 1);
		glVertex3f(x, y, z);
		glTexCoord2f(1, 0);
		glVertex3f(-x, y, z);
		glTexCoord2f(0, 0);
		glVertex3f(-x, y, -z);

		glTexCoord2f(0, 1);
		glVertex3f(x, y, -z);
		glTexCoord2f(1, 1);
		glVertex3f(x, y, z);
		glTexCoord2f(1, 0);
		glVertex3f(x, -y, z);
		glTexCoord2f(0, 0);
		glVertex3f(x, -y, -z);

		glTexCoord2f(0, 1);
		glVertex3f(x, -y, z);
		glTexCoord2f(1, 1);
		glVertex3f(x, y, z);
		glTexCoord2f(1, 0);
		glVertex3f(-x, y, z);
		glTexCoord2f(0, 0);
		glVertex3f(-x, -y, z);

		glEnd();
	}
	glPopMatrix();
	
}

void renderCuboid(float x, float y, float z) {

	glBegin(GL_LINE_LOOP);
	glColor3f(0.5, 0.5, 0);

	// Face 1 - Bottom
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

void renderTrapezoidWithoutGLU(float top, float bot1, float bot2, float y, float z) {
	glPushMatrix(); {
		glNormal3f(top, y, z);
		//glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-top / 2, -y / 2, -z / 2);
		glBegin(GL_QUADS);

		glTexCoord2f(0, 1);
		glVertex3f(bot1, 0.0f, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(bot1, 0.0f, z);
		glTexCoord2f(1, 0);
		glVertex3f(bot2, 0.0f, z);
		glTexCoord2f(0, 0);
		glVertex3f(bot2, 0.0f, 0.0f);

		glTexCoord2f(0, 1);
		glVertex3f(bot1, 0.0f, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(0.0f, y, 0.0f);
		glTexCoord2f(1, 0);
		glVertex3f(top, y, 0.0f);
		glTexCoord2f(0, 0);
		glVertex3f(bot2, 0.0f, 0.0f);

		glTexCoord2f(0, 1);
		glVertex3f(bot1, 0.0f, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(bot1, 0.0f, z);
		glTexCoord2f(1, 0);
		glVertex3f(0.0f, y, z);
		glTexCoord2f(0, 0);
		glVertex3f(0.0f, y, 0.0f);

		glTexCoord2f(0, 1);
		glVertex3f(top, y, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(top, y, z);
		glTexCoord2f(1, 0);
		glVertex3f(0.0f, y, z);
		glTexCoord2f(0, 0);
		glVertex3f(0.0f, y, 0.0f);

		glTexCoord2f(0, 1);
		glVertex3f(top, y, 0.0f);
		glTexCoord2f(1, 1);
		glVertex3f(top, y, z);
		glTexCoord2f(1, 0);
		glVertex3f(bot2, 0.0f, z);
		glTexCoord2f(0, 0);
		glVertex3f(bot2, 0.0f, 0.0f);

		glTexCoord2f(0, 1);
		glVertex3f(bot2, 0.0f, z);
		glTexCoord2f(1, 1);
		glVertex3f(top, y, z);
		glTexCoord2f(1, 0);
		glVertex3f(0.0f, y, z);
		glTexCoord2f(0, 0);
		glVertex3f(bot1, 0.0f, z);

		glEnd();
	}
	glPopMatrix();
	
}

GLuint loadTexture(LPCSTR filename) {
	//take from step 1
	GLuint texture = 0;		//texture name

	// Step 3: Initialize texture info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	// Step 4: Assign texture to polygon.
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	//Take from step 5
	DeleteObject(hBMP);
	return texture;
}

void lighting() {
	if (isLightOn) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}

	// Light 0: Red Color Ambient Light at pos (0,6,0)
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_POSITION, posA);	// set the position of the light
	glEnable(GL_LIGHT0);			// turn light 0 on

	// Light 1: Green Color Diffuse Light at pos (6,0,0)
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_POSITION, posD);	// set the position of the light
	glEnable(GL_LIGHT1);			// turn light 1 on
}

void drawInnerBody() {

	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0, -1, -0.5);
		glScalef(1.0, 1.0, 0.4);
		glRotatef(-90, 1.0, 0.0, 0.0);
		renderCylinder(0.5, 1.8, 4.0);
	}
	glPopMatrix();
}

void drawSpineJoint() {
	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 0.0);
		renderSphere(0.1);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0.15, 0.0, 0.0);
		glColor3f(1.0, 0.0, 0.0);
		renderSphere(0.05);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-0.15, 0.0, 0.0);
		glColor3f(1.0, 0.0, 0.0);
		renderSphere(0.05);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0.2, 0.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glColor3f(0.0, 1.0, 1.0);
		renderTrapezoid(0.05, 0.025, 0.2);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-0.2, 0.0, 0.0);
		glRotatef(-90, 0.0, 1.0, 0.0);
		glColor3f(0.0, 1.0, 1.0);
		renderTrapezoid(0.05, 0.025, 0.2);
	}
	glPopMatrix();
}

void drawSpine() {
	glPushMatrix();
	{
		//glTranslatef(0.0, 1.0, 0.5);
		//1
		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 1.0);
			glTranslatef(0.0, -0.4, 0.0);
			renderCubeWithoutGLU(0.1, 0.8, 0.05);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 0.4, 0.0);
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, -0.4, 0.0);
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, -0.8, 0.0);
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, -1.2, 0.0);
			drawSpineJoint();
		}
		glPopMatrix();

		//2
		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 1.0);
			glTranslatef(0.0, 1.0, 0.2);
			glRotatef(20, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.1, 0.6, 0.05);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 1.6, 0.4);
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 1.2, 0.27);
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 0.8, 0.13);
			drawSpineJoint();
		}
		glPopMatrix();

		//3
		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 1.0);
			glTranslatef(0.0, 2.1, 0.33);
			glRotatef(-10, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.1, 0.5, 0.05);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 2.6, 0.24);
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 2.3, 0.3);
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 1.95, 0.36);
			drawSpineJoint();
		}
		glPopMatrix();

		//4
		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 1.0);
			glTranslatef(0.0, 2.9, 0.21);
			glRotatef(-5, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.1, 0.3, 0.05);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 3.2, 0.18);
			drawSpineJoint();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 2.9, 0.21);
			drawSpineJoint();
		}
		glPopMatrix();

		// end of the spine
		glPushMatrix();
		{
			glTranslatef(0.0, -1.3, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			glColor3f(1.0, 0.0, 1.0);
			renderTrapezoid(0.1, 0.025, 0.4);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawScale() {
	glPushMatrix();
	{

		for (int i = 0; i <= 4; i++) {
			glPushMatrix();
			{
				glTranslatef(0.0, i*-0.2, 0.0);
				glRotatef(180, 1.0, 0.0, 0.0);
				glColor3f(1.0, 0.0, 1.0);
				renderTrapezoidWithoutGLU(0.2, 0.0, 0.0, 0.2, 0.4);
			}
			glPopMatrix();
		}
	}
	glPopMatrix();
}

void drawTopBack() {
	glPushMatrix();
	{
		//glTranslatef(0.0, -2.0, 0.0);
		glPushMatrix();
		{
			glColor3f(0.0, 0.0, 1.0);
			glTranslatef(1.0, 2.3, -0.2);
			glRotatef(-10, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.6, 0.7, 0.4);
		}
		glPopMatrix();

		glPushMatrix();
		{
			//glColor3f(0.0, 0.0, 1.0);
			glTranslatef(-1.0, 2.3, -0.2);
			glRotatef(-10, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.6, 0.7, 0.4);
		}
		glPopMatrix();

		glPushMatrix();
		{
			//glColor3f(0.0, 0.0, 1.0);
			glTranslatef(0.9, 1.0, -0.2);
			glRotatef(20, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.5, 0.6, 0.4);
		}
		glPopMatrix();

		glPushMatrix();
		{
			//glColor3f(0.0, 0.0, 1.0);
			glTranslatef(-0.9, 1.0, -0.2);
			glRotatef(20, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.5, 0.6, 0.4);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 0.0, 1.0);
			glTranslatef(0.2, 2.3, 0.05);
			glRotatef(-10, 1.0, 0.0, 0.0);
			glRotatef(-90, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(0.6, 0.1, 0.5, 0.4, 0.35);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 0.0, 1.0);
			glTranslatef(-0.2, 2.3, 0.05);
			glRotatef(-10, 1.0, 0.0, 0.0);
			glRotatef(90, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(0.6, 0.1, 0.5, 0.4, 0.35);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 0.0);
			glTranslatef(0.9, 2.0, 0.5);
			glRotatef(-10, 1.0, 0.0, 0.0);
			renderTrapezoidWithoutGLU(1.0, 0.0, 0.6, 1.5, 0.35);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 0.0);
			glTranslatef(-0.9, 2.0, 0.5);
			glRotatef(-10, 1.0, 0.0, 0.0);
			renderTrapezoidWithoutGLU(1.0, 0.4, 1.0, 1.5, 0.35);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.7, 2.5, 0.7);
			glRotatef(-10, 1.0, 0.0, 0.0);
			glRotatef(-90, 0.0, 1.0, 0.0);
			drawScale();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-0.7, 2.5, 0.7);
			glRotatef(-10, 1.0, 0.0, 0.0);
			glRotatef(-90, 0.0, 1.0, 0.0);
			drawScale();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(1.8, 1.5, -0.2);
			renderTrapezoidWithoutGLU(1.2, 0.0, 0.3, 1.0, 0.8);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(-1.8, 1.5, -0.2);
			renderTrapezoidWithoutGLU(1.2, 0.9, 1.2, 1.0, 0.8);
		}
		glPopMatrix();
		
	}
	glPopMatrix();
}

void drawHeart() {
	glPushMatrix(); 
	{
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(0.0, 1.9, -1.5);
		glScalef(2.3, 0.7, 1.0);
		//glRotatef(180, 0.0, 0.0, 1.0);
		renderPrism(0.25, 0.5, 3);
		//renderTrapezoidWithoutGLU(0.5, 0.08, 0.42, 0.3, 0.4);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(0.0, 1.72, -1.5);
		glScalef(2.3, 0.7, 1.0);
		glRotatef(180, 0.0, 0.0, 1.0);
		renderPrism(0.25, 0.5, 3);
		//renderTrapezoidWithoutGLU(0.5, 0.08, 0.42, 0.3, 0.4);
	}
	glPopMatrix();
}

void drawChest() {
	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 1.0);
		glScalef(1.3, 0.7, 1.0);
		glTranslatef(0.9, 3.5, -1.5);
		glRotatef(22.5, 0.0, 0.0, 1.0);
		renderPrism(1, 1, 8);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 1.0);
		glScalef(1.3, 0.7, 1.0);
		glTranslatef(-0.9, 3.5, -1.5);
		glRotatef(22.5, 0.0, 0.0, 1.0);
		renderPrism(1, 1, 8);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 0.0);
		glScalef(1.0, 0.4, 1.0);
		glTranslatef(0.0, 7.5, -1.5);
		glRotatef(180, 0.0, 0.0, 1.0);
		renderPrism(0.7, 1, 3);
	}
	glPopMatrix();
	
}

void drawRibs() {
	glPushMatrix();
	{
		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 1.0);
			glTranslatef(0.8, 1.7, -0.8);
			glRotatef(-27, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(2.4, 0.5, 2.2, 1, 0.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 1.0);
			glTranslatef(-0.8, 1.7, -0.8);
			glRotatef(27, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(2.4, 0.2, 1.9, 1, 0.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(0.3, 1.3, -1.15);
			glRotatef(-24, 0.0, 1.0, 0.0);
			glRotatef(30, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(0.5, 0.8, 0.8, 1.2, 0.3);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(-0.3, 1.3, -1.15);
			glRotatef(-156, 0.0, 1.0, 0.0);
			glRotatef(30, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(0.5, 0.8, 0.8, 1.2, 0.3);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawCoreDetail1() {
	for (int i = 0; i < 6; i++) {
		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 0.0);
			glTranslatef(-0.1, i*-0.2, 0.0);
			glRotatef(-10, 1.0, 0.0, 0.0);
			glRotatef(90, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(0.4, -0.1, 0.3, 0.2, 0.1);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(1.0, 0.0, 0.0);
			glTranslatef(0.1, i * -0.2, 0.0);
			glRotatef(-10, 1.0, 0.0, 0.0);
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(90, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(0.4, -0.1, 0.3, 0.2, 0.1);
		}
		glPopMatrix();
	}


}

void drawCore6Packs() {
	glPushMatrix();
	{
		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 1.0);
			glScalef(3.5, 1.5, 1.0);
			glTranslatef(0.0, 0.6, -0.9);
			glRotatef(45, 0.0, 0.0, 1.0);
			renderCubeWithoutGLU(0.3,0.3,0.2);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(1.0, 1.0, 0.0);
			glTranslatef(0.0, 0.0, -1.1);
			glRotatef(180, 0.0, 1.0, 0.0);
			glScalef(3.5, 1.5, 1.0);
			glTranslatef(0.0, 0.6, 0.0);
			glRotatef(45, 0.0, 0.0, 1.0);
			renderTrapezoid(0.4, 0.1, 0.3);
		}
		glPopMatrix();

		// 6 packs
		//1
		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 1.0);
			glTranslatef(-0.7, 0.28, -0.5);
			glRotatef(-16, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(1.3, 0.2, 1.4, 0.3, 1.0);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 1.0);
			glTranslatef(0.7, 0.28, -0.5);
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(-16, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(1.3, 0.2, 1.4, 0.3, 1.0);
		}
		glPopMatrix();

		//2
		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 1.0);
			glTranslatef(-0.6, -0.15, -0.5);
			glRotatef(-16, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(1.2, 0.2, 1.3, 0.3, 1.0);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 1.0);
			glTranslatef(0.6, -0.15, -0.5);
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(-16, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(1.2, 0.2, 1.3, 0.3, 1.0);
		}
		glPopMatrix();

		//3
		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 1.0);
			glTranslatef(-0.5, -0.6, -0.5);
			//glTranslatef(-1, -0.8, 0.0);
			glRotatef(-16, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(1.1, 0.4, 1.2, 0.3, 1.0);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 1.0);
			//glTranslatef(1, -0.8, 0.0);
			glTranslatef(0.5, -0.6, -0.5);
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(-16, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(1.1, 0.4, 1.2, 0.3, 1.0);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.0, 0.15, -1.1);
			drawCoreDetail1();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawPelvis() {

		glPushMatrix();
		{
			glColor3f(0.0, 0.0, 1.0);
			glTranslatef(-0.75, -1.0, -0.45);
			glRotatef(-16, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(1.5, 0.4, 1.6, 0.3, 1.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 0.0, 1.0);
			glTranslatef(0.75, -1.0, -0.45);
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(-16, 0.0, 0.0, 1.0);
			renderTrapezoidWithoutGLU(1.5, 0.4, 1.6, 0.3, 1.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 0.0, 1.0);
			glTranslatef(0.0, -1.6, -0.45);
			renderTrapezoidWithoutGLU(0.5, 0.2, 0.3, 0.8, 1.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.2, 0.2, 0.2);
			glTranslatef(1.0, -1.5, -0.5);
			glRotatef(-90, 0.0, 1.0, 0.0);
			renderCylinder(0.1, 0.1, 2.0);
		}
		glPopMatrix();


}

void drawShoulder() {
	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(-2.0, 3.2, -0.5);
		glRotatef(180, 1.0, 0.0, 0.0);
		renderTrapezoidWithoutGLU(2.5, 2.0, 2.0, 0.3, 1.5);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(2.0, 3.2, -0.5);
		glRotatef(180, 1.0, 0.0, 0.0);
		renderTrapezoidWithoutGLU(2.5, 0.5, 0.5, 0.3, 1.5);
	}
	glPopMatrix();
}

void drawNeck() {
	glPushMatrix();
	{
		glTranslatef(0.0, 3.2, -0.5);
		for (int i = 0; i <= 23; i++) {
			glPushMatrix();
			{
				glRotatef(i * 15, 0.0, 1.0, 0.0);
				glTranslatef(0.1, 0.0, 0.0);
				glRotatef(-90, 1.0, 0.0, 0.0);
				glColor3f(1.0, 0.0, 1.0);
				renderTrapezoidWithoutGLU(1.2, 0.0, 0.0, 0.4, 0.4);
			}
			glPopMatrix();
		}
	}
	glPopMatrix();

}

void drawInnerBodyStructure() {
	glPushMatrix();
	{
		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(0.0, 2.52, -0.5);
			renderTrapezoidWithoutGLU(5, 0.2, 4.8, 1, 1.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(0.0, 1.5, -0.5);
			renderTrapezoidWithoutGLU(4.6, 0.8, 3.8, 1, 1.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(0.0, 0.0, -0.5);
			renderTrapezoidWithoutGLU(3, 0.4, 2.6, 2, 1.5);
		}
		glPopMatrix();

		//hip
		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(0.0, -1.5, -0.5);
			renderTrapezoidWithoutGLU(2.2, 0.9, 1.3, 1, 1.5);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawStructureHead() {
	//glPushMatrix();
	//{
	//	glColor3f(1.0, 0.0, 0.0);
	//	//glTranslatef(0.0, 5.2, 0.0);
	//	renderCubeWithoutGLU(0.55, 0.35, 0.55);
	//}
	//glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 0.0);
		//glTranslatef(0.0, -0.3, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(180, 1.0, 0.0, 0.0);
		//renderPrismCylinder(0.75, 0.55, 0.7, 6);
		renderTrapezoidWithoutGLU(1.1, 0.2, 0.9, 0.9, 1.1);
	}
	glPopMatrix();

}

void drawEye() {
	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0, 0.2, -0.32);
		glRotatef(30, 1.0, 0.0, 0.0);
		renderCubeWithoutGLU(0.3, 0.1, 0.2);
	}
	glPopMatrix();
}

void drawNoseAndMouth() {
	glPushMatrix();
	{
		glColor3f(0.0, 1.0, 1.0);
		glTranslatef(0.0, -0.25, -0.6);
		//glRotatef(-30, 0.0, 1.0, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		//renderPrismCylinder(0.75, 0.55, 0.7, 6);
		renderTrapezoidWithoutGLU(1.1, 0.2, 0.9, 0.3, 0.3);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 0.0);
		glTranslatef(0.0, 0.24, -0.52);
		//glRotatef(-30, 0.0, 1.0, 0.0);
		glRotatef(15, 1.0, 0.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(90, 0.0, 0.0, 1.0);
		renderTrapezoidWithoutGLU(0.6, -0.1, -0.1, 0.3, 0.7);
	}
	glPopMatrix();
}

void drawEar() {
	glPushMatrix();
	{	
		glTranslatef(-0.63, 0.0, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		renderCylinder(0.15, 0.15, 1.3);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.0, 0.0, 1.0);
		glScalef(1.0, 1.0, 0.5);
		glTranslatef(-0.6, 0.1, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		renderPrism(0.7, 0.05, 3);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(0.0, 0.0, 1.0);
		glScalef(1.0, 1.0, 0.5);
		glTranslatef(0.6, 0.1, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		renderPrism(0.7, 0.05, 3);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glScalef(1.0, 0.5, 0.5);
		glTranslatef(-0.6, -0.5, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		renderCylinder(0.35, 0.35, 1.2);
	}
	glPopMatrix();
}

void drawHead() {

	glPushMatrix();
	{
		//glTranslatef(hx, hy, hz);
		glRotatef(hAngle, 0.0, 1.0, 0.0);

		glPushMatrix();
		{
			glTranslatef(0.0, 4.85, 0.1);

			GLuint textureArr[3];
			textureArr[0] = loadTexture("textures/eyetest.bmp");
			drawEye();
			glDeleteTextures(1, &textureArr[0]);

			textureArr[1] = loadTexture("textures/metal2.bmp");
			drawNoseAndMouth();
			drawStructureHead();
			drawEar();
			glDeleteTextures(1, &textureArr[1]);

			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}
	glPopMatrix();
	
}

void drawBody() {

	glPushMatrix();
	{
		GLuint textureArr[3];
		textureArr[0] = loadTexture("textures/steel32.bmp");

		//glScalef(4.0, 4.0, 4.0);
		glTranslatef(0.0, 1.0, 0.5);
		//glTranslatef(0.0, -5.0, 0.0);
	

		glDeleteTextures(1, &textureArr[0]);
		textureArr[1] = loadTexture("textures/darksteel32.bmp");
		drawInnerBody();
		glDeleteTextures(1, &textureArr[1]);

		textureArr[2] = loadTexture("textures/metal2.bmp");
		drawCore6Packs();
		drawTopBack();
		drawRibs();
		drawPelvis();
		drawShoulder();
		drawNeck();
		//drawInnerBodyStructure();
		drawHeart();
		drawSpine();
		drawChest();


		glDeleteTextures(1, &textureArr[2]);

		glDisable(GL_TEXTURE_2D);





		// Step5: Remove texture info.
	}
	glPopMatrix();


}

void drawLeftArm() {}
void drawRightArm() {}

void drawLeftLeg() {}
void drawRightLeg() {}

void summonGgBot() {
	glPushMatrix();
	{
		glPushMatrix();
		{
			glRotatef(180, 0.0, 1.0, 0.0);
			drawHead();
			drawBody();
		}
		glPopMatrix();


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
		renderCubeWithoutGLU(0.75, 0.75, 0.75);
		glPopMatrix();

		// body 
		glPushMatrix();
		renderCubeWithoutGLU(1, 2, 0.75);
		glPopMatrix();

		// left hand
		glPushMatrix();
		glTranslatef(-1.85, -0.5, 0);
		renderCubeWithoutGLU(0.5, 2.5, 0.5);
		glPopMatrix();

		// right hand
		glPushMatrix();
		glTranslatef(1.85, -0.5, 0);
		renderCubeWithoutGLU(0.5, 2.5, 0.5);
		glPopMatrix();

		// left leg
		glPushMatrix();
		glTranslatef(-0.65, -5.5, 0);
		renderCubeWithoutGLU(0.5, 3.5, 0.75);
		glPopMatrix();

		// right leg
		glPushMatrix();
		glTranslatef(0.65, -5.5, 0);
		renderCubeWithoutGLU(0.5, 3.5, 0.75);
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
		glTranslatef(tX, tY, tZ);
		//glRotatef(tX, 0, 1, 0);	// test rotation, look cool mah
		//tX += tSpeed;			// press f to stop rotate, press s again to start rotate

		//robotStructure();

		glColor3f(1.0, 1.0, 0.0);
		summonGgBot();

	}
	glPopMatrix();
}

void test2() {	// delete all if u want to test here 

	glPushMatrix();
	{
		glColor3f(1.0, 1.0, 0.0);

		glTranslatef(0, 0, -3);
		renderDisk(0.0f, 1.0f, 5, 10);	// pentagon

		glTranslatef(0, 2, 0);
		renderDisk(0.2f, 1.0f, 20, 10);	// disk

		glTranslatef(0, 2, 0);
		renderDisk(0.7f, 1.0f, 20, 10);	// donut disk

		glTranslatef(0, 2, 0);
		renderPartialDisk(0.0f, 1.0f, 20, 10, 0, 180);	// half disk
	}
	glPopMatrix();

	glPushMatrix();
	{
		glColor3f(1.0, 0.0, 1.0);
		renderPrism(2, 3, 5);		// penta prism

		glTranslatef(0, -5, 0);
		renderCuboid(2, 5);			// cuboid

		glTranslatef(4, 0, 0);
		renderCylinder(2, 2, 3);	// cylinder

		glTranslatef(-8, 0, 4);
		renderCone(2, 3);			// cone

		glTranslatef(0, 4, 0);
		renderPyramid(2, 3);		// pyramid

		glTranslatef(9, 1, 0);
		renderTrapezoid(1, 3, 3);	// trapezoid

		glTranslatef(-4, 4, 0);
		renderPrism(2, 3, 3);		// tri prism
		glPopMatrix();
	}
}

void test3() {	// delete all if u want to test here 
	//drawScale();
	//drawCoreDetail1();
	////drawNeck();
	//glMaterialfv(GL_FRONT, GL_AMBIENT, ambM);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);
	//glMaterialfv(GL_FRONT, GL_SHININESS, difM);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, difM);
	//glMaterialfv(GL_FRONT, GL_EMISSION, difM);
	renderTrapezoidWithoutGLU(3, 1.1, 1.9, 5, 1);
}

void display()
{

	// project initialization
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		projection();
		lighting();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//*****mouse movement start***** 
		glTranslatef(0.0, 0.0, perspecZoomLevel);
		glRotatef(mouseXRotate, 1.0, 0.0, 0.0);
		glRotatef(mouseYRotate, 0.0, 1.0, 0.0);
		glRotatef(mouseZRotate, 0.0, 0.0, 1.0);
		//*****mouse movement end*****

		glPushMatrix();
		{
			glTranslatef(lax, lay, laz);
			renderSphere(0.2);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(ldx, ldy, ldz);
			renderSphere(0.2);
		}
		glPopMatrix();
	}

	switch (qNo) {
	case 1:
		//drawBody();
		//drawHead();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);
		glMaterialfv(GL_FRONT_FACE, GL_DIFFUSE, difM);
		summonGgBot();
		break;
	case 2:
		test1();	// main robot structure here for ref
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