#include <Windowsx.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#define WINDOW_TITLE "GgBot"

int qNo = 1;
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
int r = 1;

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

void drawHead() {}
void drawBody() {}

void drawLeftArm() {}
void drawRightArm() {}

void drawLegScrew(float h) {
	glRotatef(90, 0, 1, 0);
	renderCylinder(1, 1, h);
	renderPrism(1.5 * r, 1 * r, 8);
	glTranslatef(0, 0, h);
	renderPrism(1.5 * r, 1 * r, 8);
}

void drawLegUpperFemur() {
	glPushMatrix();
	{
		glRotatef(90, 1, 0, 0);

		renderCylinder(r, r, 100 * r);

		glTranslatef(2, 0, 0);
		renderCylinder(r, r, 100 * r);

		glTranslatef(0, 2, 0);
		renderCylinder(r, r, 100 * r);

		glTranslatef(-2, 0, 0);
		renderCylinder(r, r, 100 * r);
	}
	glPopMatrix();
}

void drawLegUpperInnerThigh() {
	glPushMatrix();
	//glTranslatef(-3, 0, -2.75);
	//glScalef(0.1, 0.1, 0.1);
	{

		// inner thigh top joint
		glPushMatrix();
		{
			glColor3f(1, 0, 0);		// red
			glTranslatef(-1, 5.25, 3.25);
			glRotatef(90, 0, 1, 0);
			renderPrism(6.5 * r, 8.5 * r, 5);
		}
		glPopMatrix();

		// inner wire shieldings
		glPushMatrix();
		{
			glColor3f(0, 1, 1);
			glTranslatef(3.2, -2.5, 3.8);
			glRotatef(90, 1, 0, 0);
			renderPrism(6.5 * r, 95 * r, 5);
		}
		glPopMatrix();

		// inner wire connections
		glPushMatrix();
		{
			//glTranslatef(9, -10, -5);
			glColor3f(1, 1, 0);
			//glColor3f(87/255, 90/255, 95/255);	// rgb(87, 90, 95)

			drawLegUpperFemur();

			glTranslatef(4.5, 0, 0);
			drawLegUpperFemur();

			glTranslatef(0, 0, 4.5);
			drawLegUpperFemur();

			glTranslatef(-4.5, 0, 0);
			drawLegUpperFemur();
		}
		glPopMatrix();

		// inner thigh bottom joint
		glPushMatrix();
		{
			glColor3f(1, 0, 0);		// red
			glTranslatef(-1, -105.25, 3.25);
			glRotatef(90, 0, 1, 0);

			glPushMatrix();
			glRotatef(180, 0, 0, 1);
			renderPrism(6.5 * r, 8.5, 5);
			glPopMatrix();

		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLegUpperConnectBody() {

	// connection plate and joints
	glPushMatrix();
	{
		//glRotatef(90, 1, 0, 0);
		//glScalef(2.4, 2.4, 2.4);

		glPushMatrix();
		{
			glTranslatef(-4, 8, 2.5);
			glPushMatrix();
			{
				glTranslatef(1, 0, 5);
				drawLegScrew(2);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(1, -5, 5);
				drawLegScrew(2);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(1, -2.5, 0);
				drawLegScrew(2);
			}
			glPopMatrix();
		}
		glPopMatrix();

		glTranslatef(-4, 0, 0);
		// might delete after connect to body (template for now)
		renderCuboidWithoutGLU(r, 10 * r, 10 * r);

	}
	glPopMatrix();

}

void drawLegUpperArmor() {

	glPushMatrix();
	{
		glPushMatrix();
		{
			//glScalef(2, 2, 2);
			drawLegUpperConnectBody();
			//renderCuboidWithoutGLU(1, 1, 1);
		}
		glPopMatrix();
	}
	glPopMatrix();

}

void drawLegUpper() {

	glPushMatrix();
	{
		glScalef(0.03, 0.03, 0.03);
		//glRotatef(tX, 0, 1, 0);		// test rotation, look cool mah
		//tX += tSpeed;					// press s to stop rotate, press f again to start rotate

		// connection with body
		glPushMatrix();
		{
			glTranslatef(12, -8.5, -13.5);
			glRotatef(90, 1, 0, 0);
			glScalef(2.4, 2.4, 2.4);
			drawLegUpperConnectBody();
		}
		glPopMatrix();

		// thigh 
		glPushMatrix();
		{
			glTranslatef(12, -30, -10);	// translate to centre
			//glTranslatef(-9.2, 0, -10);

			// pink penta prism top joint
			glPushMatrix();
			{
				glTranslatef(-1, 13, 10);
				glRotatef(180, 1, 0, 0);
				glRotatef(90, 0, 1, 0);
				renderPrism(6.5 * r, 20, 5);;
			}
			glPopMatrix();

			// pink penta prism bottom joint
			glPushMatrix();
			{
				glTranslatef(-1, -113, 10);
				glRotatef(90, 0, 1, 0);
				renderPrism(6.5 * r, 20, 5);;
			}
			glPopMatrix();

			// inners of thigh
			glPushMatrix();
			{
				drawLegUpperInnerThigh();
				glTranslatef(12, 0, 0);
				drawLegUpperInnerThigh();

				glRotatef(180, 0, 1, 0);
				glTranslatef(-6, 0, -20);
				drawLegUpperInnerThigh();
				glTranslatef(12, 0, 0);
				drawLegUpperInnerThigh();

				//drawLegInnerThigh();		// angle, weird
				//glRotatef(72, 0, 1, 0);
				//drawLegInnerThigh();
				//glRotatef(72, 0, 1, 0);
				//drawLegInnerThigh();
				//glRotatef(72, 0, 1, 0);
				//drawLegInnerThigh();
				//glRotatef(72, 0, 1, 0);
				//drawLegInnerThigh();
			}
			glPopMatrix();

		}
		glPopMatrix();
	}
	glPopMatrix();

	// upper leg Armor
	glPushMatrix();
	{
		//drawLegUpperArmor();
	}
	glPopMatrix();
}

void drawLegKnee() {}

void drawLegLower() {}

void drawLegFoot() {}

void drawLeftLeg() {}

void drawRightLeg() {

	// pelvis
	// hip joint (connection)

	// thigh (femur)
		// inner thigh
		// back thigh (muscle)

	// knee (connection between femur and tibia)

	// shin (tibia)
		// calf (muscle)
	// foot

	glPushMatrix();
	{
		//glTranslatef(0.35, -0.5, 0);
		//glRotatef(90, 0, 1, 0);

		//glScalef(0.03, 0.03, 0.03);
		drawLegUpper();
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
		//glRotatef(tX, 0, 1, 0);		// test rotation, look cool mah
		//tX += tSpeed;					// press s to stop rotate, press f again to start rotate

		//glPushMatrix();
		//{
		//	glTranslatef(0, 5.5, 0);	// move sphere up to head
		//	renderSphere(1.5);
		//}
		//glPopMatrix();

		//glTranslatef(tX, tY, tZ);           
		robotStructure();


		glColor3f(1.0, 1.0, 0.0);
		summonGgBot();

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

	//glScalef(0.3, 0.3, 0.3);
	glTranslatef(-0.75, 3, 0);

	drawLegUpper();

	glPopMatrix();
}

void test4() {
	glColor3f(1.0, 0.0, 1.0);

	glPushMatrix();

	drawLegUpperArmor();

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