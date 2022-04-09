#include <Windowsx.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#define WINDOW_TITLE "GgBot"

//GLUquadricObj* obj = NULL;
//GLUquadricObj* obj = gluNewQuadric();

int qNo = 1;
float stacks = 10;

// transformation for projection matrix
float ptX, ptY, prY = 0;
float ptSpeed = 0.1;

// transformation for object
float tX, tY, tZ = 0;
float tSpeed = 0.5;
float faceAngle = 0;

// set Perspective View as default
boolean isOrtho = false;
float orthoNear = -20, orthoFar = 20.0;
float perspecNear = 10, perspecFar = 20;

// mouse movements
float mouseLastX = 0.0f, mouseLastY = 0.0f;
float mouseXRotate = 0.0f, mouseYRotate = 0.0f, mouseZRotate = 0.0f;
float perspecZoomLevel = -2.0f;

// other (Seperate to your own sections too)
int r = 1;


//Hand
bool armUpBool = false;
bool armDownBool = false;
bool armLeftBool = false;
bool armRightBool = false;
float armRSpeed = 0.1;
float armRup = 0.01, armRup1 = 0.01, armRsword = 0.01; //arm rotation variable, need reset for space
char temp = 'A';

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
		perspecZoomLevel -= GET_WHEEL_DELTA_WPARAM(wParam) / 150.0f;
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
		else if (wParam == 0x34) {
			qNo = 4;
		}
		else if (wParam == VK_SPACE) {
			tX = 0, tY = 0, tZ = 0;
			mouseXRotate = 0.0f, mouseYRotate = 0.0f, mouseZRotate = 0.0f, perspecZoomLevel = -2.0f;
			ptX = 0, ptY = 0, prY = 0;

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
			//if (isOrtho) {
			//	if (ptX < 1.1)
			//		ptX += ptSpeed;
			//}
			//else {
			//	prY -= ptSpeed * 15;	// perspective rotate y-axis, object look flat
			//}
			faceAngle = 270;
		}
		else if (wParam == 'D') {
			//if (isOrtho) {
			//	if (ptX > -1.1)
			//		ptX -= ptSpeed;
			//}
			//else {
			//	prY += ptSpeed * 15;	// perspective rotate y-axis, object look flat
			//}
			faceAngle = 90;
		}
		else if (wParam == 'W') {
			//if (isOrtho) {
			//	if (ptY > -1.3)
			//		ptY -= ptSpeed;
			//}
			//else {
			//	perspecZoomLevel -= 1;
			//}
			faceAngle = 0;
		}
		else if (wParam == 'S') {
			//if (isOrtho) {
			//	if (ptY < 1.3)
			//		ptY += ptSpeed;
			//}
			//else {
			//	perspecZoomLevel += 1;
			//}
			faceAngle = 180;
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
		else if (wParam == 'U') {
			armDownBool = false;
			if (wParam == temp) {
				armUpBool = false;
				temp = NULL;

			}
			else {
				armUpBool = true;
				temp = wParam;

			}

		}
		else if (wParam == 'I') {
			armUpBool = false;
			if (wParam == temp) {
				armDownBool = false;
				temp = NULL;

			}
			else {
				armDownBool = true;
				temp = wParam;

			}
		}
		else if (wParam == 'T') {
			armRightBool = false;
			if (wParam == temp) {
				armLeftBool = false;
				temp = NULL;

			}
			else {
				armLeftBool = true;
				temp = wParam;

			}

		}
		else if (wParam == 'Y') {
			armLeftBool = false;
			if (wParam == temp) {
				armRightBool = false;
				temp = NULL;

			}
			else {
				armRightBool = true;
				temp = wParam;

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

	glTranslatef(ptX, ptY, 0.0f);		// projection translation
	glRotatef(prY, 0.0, 1.0, 0.0);		// projection rotation (y-axis only)

	if (isOrtho) {
		glOrtho(-10, 10, -10, 10, orthoNear, orthoFar);
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
	gluQuadricDrawStyle(sphere, GLU_SILHOUETTE);	// will change to GLU_FILL
	gluSphere(sphere, r, 50, 50);
	gluDeleteQuadric(sphere);
}

void renderDisk(float inR, float outR, float slices, float loops) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);	// will change to GLU_FILL		// GLU_LINE, GLU_SILHOUETTE, GLU_POINT
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
	gluQuadricDrawStyle(obj, GLU_LINE);	// will change to GLU_FILL
	//gluQuadricDrawStyle(obj, GLU_FILL);	// will change to GLU_FILL
	gluCylinder(obj, baseR, topR, h, slices, stacks);
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

void renderPrism(float l, float h, float slices) { // slice 3 - triprism, 5 - pentaprism..
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

void renderCuboidWithoutGLU(float x, float y, float z) {

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

void renderTrapezoidWithoutGLU(float top, float bot1, float bot2, float y, float z) {
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

		//// left hand connection point
		//glPushMatrix();
		//glTranslatef(-1.2, 1.5, 0);
		//renderSphere(0.25);
		//glPopMatrix();

		//// right hand connection point
		//glPushMatrix();
		//glTranslatef(1.2, 1.5, 0);
		//renderSphere(0.25);
		//glPopMatrix();
	}
	glPopMatrix();

	// leg spheres for reference

	renderSphere(0.1);	// centre sphere
	glPushMatrix();
	glTranslatef(0, -7, 0);
	renderSphere(0.1);	// bottom sphere middle
	glPopMatrix();
	//glPushMatrix();
	//glTranslatef(-1.1, 0, 0);
	//renderSphere(0.1);	// left sphere 
	//glPopMatrix();
	//glPushMatrix();
	//glTranslatef(1.1, 0, 0);
	//renderSphere(0.1);	// right sphere 
	//glPopMatrix();

}

void robotStructure1() {


	glPushMatrix();
	glTranslatef(0, 2, 0);	// move to centre on window
	{
		//glScalef(0.10, 0.10, 0.10);

		//head
		glPushMatrix();
		glTranslatef(0, 3.5, 0);
		renderCubeWithoutGLU(0.75, 0.75, 0.75);
		glPopMatrix();

		//body 
		glPushMatrix();
		renderCubeWithoutGLU(1, 2, 0.75);
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

		/*
		// left hand connection point
		glPushMatrix();
		glTranslatef(-1.2, 1.5, 0);
		renderSphere(0.25);
		glPopMatrix();
		*/

		/*
		// right hand connection point
		glPushMatrix();
		glTranslatef(1.2, 1.5, 0);
		renderSphere(0.25);
		glPopMatrix();
		*/
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

void drawTopBack() {
	glPushMatrix();
	{
		//glTranslatef(0.0, -2.0, 0.0);
		glPushMatrix();
		{
			glColor3f(0.0, 0.0, 1.0);
			glTranslatef(0.6, 2.5, 0.05);
			glRotatef(-10, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.4, 0.8, 0.2);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 0.0, 1.0);
			glTranslatef(-0.6, 2.5, 0.05);
			glRotatef(-10, 1.0, 0.0, 0.0);
			renderCubeWithoutGLU(0.4, 0.8, 0.2);
		}
		glPopMatrix();

	}
	glPopMatrix();
}

void drawInnerBodyStructure() {
	glPushMatrix();
	{
		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(-2.5, 2.02, -1.25);
			renderTrapezoidWithoutGLU(5, 0.2, 4.8, 1, 1.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(-2.3, 1, -1.25);
			renderTrapezoidWithoutGLU(4.6, 0.8, 3.8, 1, 1.5);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(-1.5, -1, -1.25);
			renderTrapezoidWithoutGLU(3, 0.4, 2.6, 2, 1.5);
		}
		glPopMatrix();

		//hip
		glPushMatrix();
		{
			glColor3f(0.0, 1.0, 0.0);
			glTranslatef(-1.1, -2, -1.25);
			renderTrapezoidWithoutGLU(2.2, 0.9, 1.3, 1, 1.5);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawHead() {}
void drawBody() {
	glPushMatrix();
	{
		//glScalef(6.0, 6.0, 6.0);
		glTranslatef(0.0, 1.0, 0.5);
		//glTranslatef(0.0, -1.0, 0.0);
		drawSpine();
		drawTopBack();
		drawInnerBodyStructure();
	}
	glPopMatrix();
}



void drawLeftArm() {

	if (armUpBool) {
		if (armRup1 <= 50) {
			armRup1 += armRSpeed;

		}
		else if (armRup <= 15 || armRup1 <= 110) {
			if (armRup <= 15) {
				armRup += armRSpeed / 2;
				armRup1 += armRSpeed / 2;
			}
			else {
				armRup1 += armRSpeed / 2;

			}


		}
	}
	else if (armDownBool) {
		if (armRup1 >= 50) {
			armRup1 -= armRSpeed;

		}
		else if (armRup1 >= 0) {
			if (armRup >= 0) {
				armRup -= armRSpeed / 2;
				armRup1 -= armRSpeed / 2;
			}
			else {
				armRup1 -= armRSpeed;
			}


		}
	}

	if (armLeftBool) {
		if (armRsword <= 86) {
			armRsword += armRSpeed;

		}
	}
	else if (armRightBool) {
		if (armRsword >= 0)
			armRsword -= armRSpeed;
	}

	glPushMatrix();
	glTranslatef(0, 2, 0);	// move to centre on window

	//left hand
		//white
	glPushMatrix();
	glTranslatef(-1.85, 1.8, 0);
	renderCubeWithoutGLU(0.8, 0.4, 0.80);
	glPopMatrix();

	//below white
	glPushMatrix();
	glTranslatef(-1.85, 1.2, -0.6);
	renderCubeWithoutGLU(0.8, 0.2, 0.2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.85, 1.2, 0.6);
	renderCubeWithoutGLU(0.8, 0.2, 0.2);
	glPopMatrix();

	//black
	glPushMatrix();
	glTranslatef(-2.55, 1.8, 0);
	renderCubeWithoutGLU(0.3, 0.2, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.65, 1.8, 0);
	renderCubeWithoutGLU(0.6, 0.2, 1);
	glPopMatrix();

	//Connection
	glPushMatrix();
	glTranslatef(-2.15, 1.4, 0);
	glRotatef(90, 0, 1, 0);
	renderCylinder(0.2, 0.2, 1.5);
	glPopMatrix();



	//Upper arm
	glPushMatrix();
	glTranslatef(-1.85, 1.4, 0);
	glRotatef(armRup, 1, 0, 0);
	glTranslatef(1.85, -1.4, 0);
	//white inner hand
	glPushMatrix();
	glTranslatef(-1.85, 1.4, 0);
	glRotatef(90, 1, 0, 0);
	renderTrapezoid(0.4, 0.2, 1.6);
	glPopMatrix();

	//Lower arm
	glPushMatrix();
	glTranslatef(-1.85, 0.0, 0);
	glRotatef(armRup1, 1, 0, 0);
	glRotatef(armRsword, 1, 0, 1);	// a
	glTranslatef(1.85, 0.0, 0);
	//black inner hand
	glPushMatrix();
	glTranslatef(-1.85, 0.0, 0);
	glRotatef(90, 1, 0, 0);
	renderTrapezoid(0.4, 0.6, 1.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.85, -1.2, 0);
	glRotatef(90, 1, 0, 0);
	renderTrapezoid(0.6, 0.8, 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.85, -1.4, 0);
	glRotatef(90, 1, 0, 0);
	renderTrapezoid(0.8, 0.6, 0.6);
	glPopMatrix();
	//end black inner hand

	//yellow part
	glPushMatrix();
	glTranslatef(-2, -0.6, 0);
	glRotatef(90, -1, -0.5, 0);
	renderTrapezoid(0.3, 0.02, 0.8);
	glPopMatrix();

	//palm
	glPushMatrix();
	glTranslatef(-2.25, -2.2, 0);
	glRotatef(90, 0, 1, 0);
	renderTrapezoid(0.2, 0.6, 0.2);
	glPopMatrix();

	//finger
	glPushMatrix();
	glTranslatef(-1.95, -2.2, 0);
	renderCubeWithoutGLU(0.1, 0.4, 0.4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.95, -2.2, -0.5);
	glRotatef(45, 1, 0, 0);
	renderCubeWithoutGLU(0.1, 0.4, 0.1);
	glPopMatrix();

	//1st part finger
	glPushMatrix();
	glTranslatef(-1.95, -2.8, -0.3);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.95, -2.8, -0.1);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.95, -2.8, 0.3);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.95, -2.8, 0.1);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();

	//2nd part
	glPushMatrix();
	glTranslatef(-1.95, -3.2, -0.3);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.95, -3.2, -0.1);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.95, -3.2, 0.3);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.95, -3.2, 0.1);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();

	glPopMatrix(); //left lowerarm pop
	glPopMatrix(); //left upperarm pop

	glPopMatrix(); // translate up 2 via y


}

void drawRightArm() {

	glPushMatrix();
	glTranslatef(0, 2, 0);	// move to centre on window
	//right hand
		//white
	glPushMatrix();
	glTranslatef(1.85, 1.8, 0);
	renderCubeWithoutGLU(0.8, 0.4, 0.80);
	glPopMatrix();

	//below white
	glPushMatrix();
	glTranslatef(1.85, 1.2, -0.6);
	renderCubeWithoutGLU(0.8, 0.2, 0.2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.85, 1.2, 0.6);
	renderCubeWithoutGLU(0.8, 0.2, 0.2);
	glPopMatrix();

	//black
	glPushMatrix();
	glTranslatef(2.55, 1.8, 0);
	renderCubeWithoutGLU(0.3, 0.2, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.65, 1.8, 0);
	renderCubeWithoutGLU(0.6, 0.2, 1);
	glPopMatrix();

	//Connection
	glPushMatrix();
	glTranslatef(0.65, 1.4, 0);
	glRotatef(90, 0, 1, 0);
	renderCylinder(0.2, 0.2, 1.5);
	glPopMatrix();

	//white inner hand
	glPushMatrix();
	glTranslatef(1.85, 1.4, 0);
	glRotatef(90, 1, 0, 0);
	renderTrapezoid(0.4, 0.2, 1.2);
	glPopMatrix();

	//black inner hand
	glPushMatrix();
	glTranslatef(1.85, 0.4, 0);
	glRotatef(90, 1, 0, 0);
	renderTrapezoid(0.4, 0.6, 1.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.85, -1.2, 0);
	glRotatef(90, 1, 0, 0);
	renderTrapezoid(0.6, 0.8, 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.85, -1.4, 0);
	glRotatef(90, 1, 0, 0);
	renderTrapezoid(0.8, 0.6, 0.6);
	glPopMatrix();
	//end black inner hand

	//yellow part
	glPushMatrix();
	glTranslatef(2, -0.6, 0);
	glRotatef(90, -1, 0.5, 0);
	renderTrapezoid(0.3, 0.02, 0.8);
	glPopMatrix();


	//palm
	glPushMatrix();
	glTranslatef(2.25, -2.2, 0);
	glRotatef(90, 0, 1, 0);
	renderTrapezoid(0.6, 0.2, 0.2);
	glPopMatrix();
	//finger
	glPushMatrix();
	glTranslatef(1.95, -2.2, 0);
	renderCubeWithoutGLU(0.1, 0.4, 0.4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.95, -2.2, -0.5);
	glRotatef(45, 1, 0, 0);
	renderCubeWithoutGLU(0.1, 0.4, 0.1);
	glPopMatrix();
	//1st part finger
	glPushMatrix();
	glTranslatef(1.95, -2.8, -0.3);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.95, -2.8, -0.1);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.95, -2.8, 0.3);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.95, -2.8, 0.1);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();
	//2nd part
	glPushMatrix();
	glTranslatef(1.95, -3.2, -0.3);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.95, -3.2, -0.1);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.95, -3.2, 0.3);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.95, -3.2, 0.1);
	renderCubeWithoutGLU(0.1, 0.2, 0.1);
	glPopMatrix();


	//gun
	//glColor3f(0, 1, 0);
	//glPushMatrix();
	//glTranslatef(1.85, -1.8, 0);
	//glRotatef(90, 1, 0, 0);
	//renderCylinder(0.3, 0.3, 0.7);
	//glPopMatrix();

	////bullet
	//glColor3f(1, 0, 0);
	//glPushMatrix();
	//glTranslatef(1.85, -1.8, 0);
	//glRotatef(90, 1, 0, 0);
	//renderSphere(0.28);
	//glPopMatrix();
	//end Right hand

	glPopMatrix(); // translate up 2 via y

}




void drawLegScrew(float r, float h) {
	glRotatef(90, 0, 1, 0);
	renderCylinder(1, 1, h);
	renderPrism(1.5 * r, 1 * r, 8);
	glTranslatef(0, 0, h);
	renderPrism(1.5 * r, 1 * r, 8);
}


void drawLegInnerNerve(float r, float h) {

	// 4 nerves
	glPushMatrix();
	{
		//glTranslatef(-r, h / 2, -r);	// move to origin with 4 nerves
		glTranslatef(0, h / 2, 0);	// move to origin just 1 nerve
		glPushMatrix();
		{
			glRotatef(90, 1, 0, 0);
			glColor3f(1, 1, 0);				  // red

			renderCylinder(r, r, h);

			//glPushMatrix();
			//glTranslatef(2 * r, 0, 0);		// move right
			//renderCylinder(r, r, h);
			//glPopMatrix();

			//glPushMatrix();
			//glTranslatef(0, 2 * r, 0);		// move up
			//renderCylinder(r, r, h);
			//glPopMatrix();

			//glPushMatrix();
			//glTranslatef(2 * r, 2 * r, 0);		// move left
			//renderCylinder(r, r, h);
			//glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();

	// nerve shield (cover)
	glPushMatrix();
	{
		glColor3f(1, 0, 0);				  // red
		glTranslatef(0, (0.8 * h) / 2, 0);
		glRotatef(90, 1, 0, 0);
		renderPrism(2.5 * r, 0.8 * h, 8);
	}
	glPopMatrix();

	// penta joint (top)
	glPushMatrix();
	{
		glColor3f(0, 1, 1);
		//glTranslatef(0, 5.3, -0.4);
		glTranslatef(0, h / 2, -0.4);

		glPushMatrix();
		renderPrism(3 * r, 4 * r, 5);
		glPopMatrix();
	}
	glPopMatrix();

	// penta joint (bottom)
	glPushMatrix();
	{
		glColor3f(0, 1, 1);

		//glTranslatef(0, -5.3, -0.4);
		glTranslatef(0, -h / 2, -0.4);

		glPushMatrix();
		glRotatef(180, 0, 0, 1);
		renderPrism(3 * r, 4 * r, 5);
		glColor3f(1, 0, 0);
		renderCylinder(1.5 * r, 1.5 * r, 5 * r);
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLegInner(float h) {
	glPushMatrix();
	{
		glScalef(0.3, 0.3, 0.3);
		glRotatef(90, 0, 1, 0);

		// 4 nerves
		glPushMatrix();
		glTranslatef(-0.5, 0, -0.5);	// move to origin
		{
			glPushMatrix();
			drawLegInnerNerve(0.2, h);	// nerve 1

			glPushMatrix();
			glTranslatef(1, 0, 0);
			drawLegInnerNerve(0.2, h);	// nerve 2
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 0, 1);
			drawLegInnerNerve(0.2, h);	// nerve 3
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1, 0, 1);
			drawLegInnerNerve(0.2, h);	// nerve 4
			glPopMatrix();
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLegKneeJoint() {
	glPushMatrix();
	{
		renderPrism(r, r, 6);
		glPushMatrix();
		renderPrism(0.3 * r, 4 * r, 8);
		glTranslatef(0, 0, 3);
		renderPrism(r, r, 6);
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.875, 1.5, 0);
			renderPrism(r, r, 6);
			glPushMatrix();
			renderPrism(0.3 * r, 4 * r, 8);
			glTranslatef(0, 0, 3);
			renderPrism(r, r, 6);
			glPopMatrix();

			//glTranslatef(0.875, 1.5, 0);
			//renderPrism(r, r, 6);
			//glPushMatrix();
			//renderPrism(0.3 * r, 4 * r, 8);
			//glTranslatef(0, 0, 3);
			//renderPrism(r, r, 6);
			//glPopMatrix();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.875, -1.5, 0);
			renderPrism(r, r, 6);
			glPushMatrix();
			renderPrism(0.3 * r, 4 * r, 8);
			glTranslatef(0, 0, 3);
			renderPrism(r, r, 6);
			glPopMatrix();

			//glTranslatef(0.875, -1.5, 0);
			//renderPrism(r, r, 6);
			//glPushMatrix();
			//renderPrism(0.3 * r, 4 * r, 8);
			//glTranslatef(0, 0, 3);
			//renderPrism(r, r, 6);
			//glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLegKnee() {

	glPushMatrix();
	{
		glColor3f(0.5, 0.3, 0.5);

		//glTranslatef(0.925, -4.8, -0.275);		
		//glTranslatef(0.925, -4.6, -0.4);	// Don't delete, knee with 5 joints
		//glTranslatef(0.875, -4.3, -0.3);	// knee with 3 joints
		glRotatef(-90, 0, 1, 0);
		glRotatef(10, 0, 0, 1);
		glScalef(0.13, 0.13, 0.13);

		// connect legUpper and legLower
		drawLegKneeJoint();
		glTranslatef(3, 0, 0);
		drawLegKneeJoint();
	}
	glPopMatrix();
}

void drawLegLower(float h) {
	glPushMatrix();
	{
		glScalef(0.3, 0.3, 0.3);
		glRotatef(90, 0, 1, 0);

		// 4 nerves
		glPushMatrix();
		glTranslatef(-0.5, 0, -0.5);	// move to origin
		{
			glPushMatrix();
			drawLegInnerNerve(0.2, h);

			glPushMatrix();
			glTranslatef(1, 0, 0);
			drawLegInnerNerve(0.2, h);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0, 0, 1);
			drawLegInnerNerve(0.2, h);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1, 0, 1);
			drawLegInnerNerve(0.2, h);
			glPopMatrix();
			glPopMatrix();
		}
		glPopMatrix();

	}

	glPopMatrix();
}

void drawLegFoot() {

}

void drawLeftLeg() {
	glPushMatrix();
	{

		glPushMatrix();
		{
			glTranslatef(-0.65, -2, 0);	// move to right leg position
			drawLegInner(10);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-0.39, -3.8, -0.3);	// knee with 3 joints
			drawLegKnee();
		}
		glPopMatrix();


		glPushMatrix();
		{
			glTranslatef(-0.65, -5, 0);	// move to right leg position
			drawLegInner(7);
		}
		glPopMatrix();

		//drawLegFoot();
	}
	glPopMatrix();
}

void drawRightLeg() {

	glPushMatrix();
	{

		glPushMatrix();
		{
			glTranslatef(0.65, -2, 0);	// move to right leg position
			drawLegInner(10);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.91, -3.8, -0.3);	// knee with 3 joints
			drawLegKnee();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.65, -5, 0);	// move to right leg position
			drawLegInner(7);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.85, -6.3, -0.25);	// move to right leg position
			glScalef(0.8, 0.8, 0.8);
			drawLegKnee();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.65, -6.6, 0);	// move to right leg position
			drawLegInner(1);
		}
		glPopMatrix();

		drawLegFoot();
	}
	glPopMatrix();
}

void summonGgBot() {
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

void test1() {
	glPushMatrix();
	{
		glColor3f(0.0, 1.0, 1.0);
		glTranslatef(tX, tY, tZ);
		//glRotatef(faceAngle, 0.0f, 1.0f, 0.0f);
		//glRotatef(tX, 0, 1, 0);		// test rotation, look cool mah
		//tX += tSpeed;					// press s to stop rotate, press f again to start rotate

		//glTranslatef(tX, tY, tZ);           
		robotStructure();

		glColor3f(1.0, 1.0, 0.0);
		summonGgBot();

		//for (int i = 0; i <= 100000; i++) {
		//	//renderCylinder(1, 1, 1);
		//	renderCubeWithoutGLU(10, 10, 10);
		//}
	}
	glPopMatrix();
}

void test2() {
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

void test3() {
	glColor3f(1.0, 0.0, 1.0);

	glPushMatrix();
	//robotStructure();

	//glTranslatef(-0.75, 3, 0);

	drawLegInner(10);

	glPopMatrix();
}

void test4() {
	glColor3f(1.0, 0.0, 1.0);

	glPushMatrix();

	//drawLegKnee();
	for (int i = 0; i <= 1000; i++) {
		renderCylinder(1, 1, 1);
		//renderCubeWithoutGLU(1, 1, 1);
	}

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
		glTranslatef(0.0, 0.0, perspecZoomLevel);
		glRotatef(mouseXRotate, 1.0, 0.0, 0.0);
		glRotatef(mouseYRotate, 0.0, 1.0, 0.0);
		glRotatef(mouseZRotate, 0.0, 0.0, 1.0);
		//*****mouse movement end*****
	}
	glRotatef(faceAngle, 0.0f, 1.0f, 0.0f);


	switch (qNo) {
	case 1:
		test1();	// main robot structure here for ref
		break;
	case 2:
		test2();	// change to what u want for test
		break;
	case 3:
		test3();	// change to what u want for test
		break;
	case 4:
		test4();	// change to what u want for test
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


//void drawLeftArm() {
//	glPushMatrix();
//	{
//		glTranslatef(1.85, 2.8, 0);	// move to left upper arm position
//		drawLegInner(5);
//	}
//	glPopMatrix();
//
//	glPushMatrix();
//	{
//		glTranslatef(-1.59, 1.8, -0.3);	// knee with 3 joints
//		drawLegKnee();
//	}
//	glPopMatrix();
//
//	glPushMatrix();
//	{
//		glTranslatef(1.85, 0.85, 0);	// move to left lower arm position
//		drawLegInner(5);
//	}
//	glPopMatrix();
//}

//void drawRightArm() {
//	glPushMatrix();
//	{
//		glTranslatef(-1.85, 2.8, 0);	// move to right upper arm position
//		drawLegInner(5);
//	}
//	glPopMatrix();
//
//	glPushMatrix();
//	{
//		glTranslatef(2.11, 1.8, -0.3);	// knee with 3 joints
//		drawLegKnee();
//	}
//	glPopMatrix();
//
//	glPushMatrix();
//	{
//		glTranslatef(-1.85, 0.85, 0);	// move to right lower arm position
//		drawLegInner(5);
//	}
//	glPopMatrix();
//}