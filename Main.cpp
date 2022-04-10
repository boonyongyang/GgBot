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

//Hand
bool leftArmUpBool = false,leftArmDownBool = false;
bool rightArmUpBool = false,rightArmDownBool = false;
bool fingerUpBool = false,fingerDownBool=false;
bool armLeftBool = false,armRightBool = false;
bool shootBullet = false;
float armRSpeed = 0.1;
float	leftArmRup = 0.01, leftArmRup1 = 0.01, 
		rightArmRup = 0.01, rightArmRup1 = 0.01, 
		fingerRup =0.01, fingerRup1=0.01,
	armRsword = 0.01; //arm rotation variable, need reset for space
char temp = 'A';
float bullet = 0.0;
//extra feature
bool boolHI = false;//example
bool boolWeapon = false;
bool boolSword = false;

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
			ptX = 0, ptY = 0, prY = 0;
			leftArmRup = 0.01, leftArmRup1 = 0.01,
			rightArmRup = 0.01, rightArmRup1 = 0.01,
			fingerRup = 0.01, fingerRup1 = 0.01,
			armRsword = 0.01;
			leftArmUpBool = false, leftArmDownBool = false;
			rightArmUpBool = false, rightArmDownBool = false;
			fingerUpBool = false, fingerDownBool = false;
			armLeftBool = false, armRightBool = false;
			boolHI = false;

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
		else if (wParam == 'U') {
			//left arm UP
			leftArmDownBool = false;
			if (wParam == temp) {
				leftArmUpBool = false;
				temp = NULL;

			}
			else {
				leftArmUpBool = true;
				temp = wParam;

			}

		}
		else if (wParam == 'I') {
			//left arm DOWN
			leftArmUpBool = false;
			if (wParam == temp) {
				leftArmDownBool = false;
				temp = NULL;

			}
			else {
				leftArmDownBool = true;
				temp = wParam;

			}
		}
		else if (wParam == 'T') {
			//right arm Up
			rightArmDownBool = false;
			if (wParam == temp) {
				rightArmUpBool = false;
				temp = NULL;

			}
			else {
				rightArmUpBool = true;
				temp = wParam;

			}

		}
		else if (wParam == 'Y') {
			//right arm Down
			rightArmUpBool = false;
			if (wParam == temp) {
				rightArmDownBool = false;
				temp = NULL;

			}
			else {
				rightArmDownBool = true;
				temp = wParam;

			}
		}
		else if (wParam == 'Z') {
			//finger Up
			fingerDownBool = false;
			if (wParam == temp) {
				fingerUpBool = false;
				temp = NULL;

			}
			else {
				fingerUpBool = true;
				temp = wParam;

			}

		}
		else if (wParam == 'X') {
		//finger Down
		fingerUpBool = false;
		if (wParam == temp) {
			fingerDownBool = false;
			temp = NULL;

		}
		else {
			fingerDownBool = true;
			temp = wParam;

		}
		}
		else if (wParam == 'C') {
			//right arm Up
			shootBullet = false;
			if (wParam == temp) {
				shootBullet = false;
				temp = NULL;

			}
			else {
				shootBullet = true;
				temp = 'C';

			}

		}
		else if (wParam == 'V') {
		//finger Up
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
		else if (wParam== VK_F1) {
			boolWeapon = false;
			boolHI = false;
			boolSword = false;

		}
		else if (wParam == VK_F2) {
			boolWeapon = true;
			boolHI = false;
			boolSword = false;
			leftArmUpBool = true;
			rightArmUpBool = true;
			leftArmRup = 0.01, leftArmRup1 = 0.01,
				rightArmRup = 0.01, rightArmRup1 = 0.01;
		}
		else if (wParam == VK_F3) {
		boolWeapon = false;
		fingerUpBool = true;
		boolSword = true;
		}
		else if (wParam == VK_F4) {
		boolWeapon = false;
		//boolSword
		boolHI = true;
		fingerUpBool = true;
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
	gluQuadricDrawStyle(sphere, GLU_SILHOUETTE);	// will change to GLU_FILL
	gluSphere(sphere, r, 50, 50);
	gluDeleteQuadric(sphere);
}

void renderDisk(float inR, float outR, float slices, float loops) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);	// will change to GLU_FILL		// GLU_LINE, GLU_SILHOUETTE, GLU_POINT
	//gluQuadricDrawStyle(obj, GLU_FILL);	
	gluDisk(obj, inR, outR, slices, loops);
	gluDeleteQuadric(obj);
}

void renderPartialDisk(float inR, float outR, float slices, float loops, float startAng, float endAngle) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);	// will change to GLU_FILL
	//gluQuadricDrawStyle(obj, GLU_FILL);	
	gluPartialDisk(obj, inR, outR, slices, loops, startAng, endAngle);
	gluDeleteQuadric(obj);
}

void renderPolygon(float baseR, float topR, float h, float slices) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);	// will change to GLU_FILL
	//gluQuadricDrawStyle(obj, GLU_FILL);	
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


void drawHead() {}
void drawBody() {}

void drawLeftArm() {
	if (leftArmUpBool) {
		if (leftArmRup1 <= 50) {
			leftArmRup1 += armRSpeed;

		}
		else if (leftArmRup <= 15 || leftArmRup1 <= 110) {
			if (boolWeapon == false || boolSword==true) {
				if (leftArmRup <= 15) {
					leftArmRup += armRSpeed / 2;
					leftArmRup1 += armRSpeed / 2;
				}
				else {
					leftArmRup1 += armRSpeed / 2;

				}
			}
			else {
				if (leftArmRup <= 15) {
					leftArmRup += armRSpeed / 2;
					if (leftArmRup1 <= 75)
						leftArmRup1 += armRSpeed / 2;
				}
				else {
					if (leftArmRup1 <= 75)
						leftArmRup1 += armRSpeed / 2;

				}
			}


		}
	}
	else if (leftArmDownBool) {

		if (leftArmRup1 >= 50) {
			leftArmRup1 -= armRSpeed;

		}
		else if (leftArmRup1 >= 0) {
			if (leftArmRup >= 0) {
				leftArmRup -= armRSpeed / 2;
				leftArmRup1 -= armRSpeed / 2;
			}
			else {
				leftArmRup1 -= armRSpeed;
			}


		}
	}

	if (fingerUpBool) {
		if (fingerRup1 <= 90 && fingerRup <=90) {
			if (boolHI) {
				fingerRup += armRSpeed;

			}
			else {
				fingerRup += armRSpeed;
				fingerRup1 += armRSpeed;

			}

		}
	}
	else if (fingerDownBool) {
		if (fingerRup1 >= 0) {
			fingerRup1 -= armRSpeed;

		}
		if (fingerRup >= 0) {
			fingerRup -= armRSpeed;

		}
	}

	//'V' atk action
	if (armLeftBool) {
		if (leftArmRup1 <= 50) {
			leftArmRup1 += armRSpeed;

		}
		else if (leftArmRup <= 15 || leftArmRup1 <= 110) {
			if (leftArmRup <= 15) {
					leftArmRup += armRSpeed / 2;
					if (leftArmRup1 <= 110)
						leftArmRup1 += armRSpeed / 2;
				}
				else {
					if (leftArmRup1 <= 110)
						leftArmRup1 += armRSpeed / 2;

				}
			}else if (armRsword <= 60) {
				armRsword += armRSpeed*2;
				leftArmRup1 -= armRSpeed*2;

			}
		else {
			armLeftBool = false;
			armRightBool = true;
		}
		
	}
	else if (armRightBool) {
		if (armRsword >= 0) {
			armRsword -= armRSpeed;

		}
		else {
			if (leftArmRup1 >= 50) {
				leftArmRup1 -= armRSpeed;

			}
			else if (leftArmRup1 >= 0) {
				if (leftArmRup >= 0) {
					leftArmRup -= armRSpeed / 2;
					leftArmRup1 -= armRSpeed / 2;
				}
				else {
					leftArmRup1 -= armRSpeed;
				}


			}
			if (leftArmRup1 <= 0) {
				armRightBool = false;

			}
		}

	}

	if (shootBullet) {
		bullet -= armRSpeed/2;
		if (bullet >= 10) {
			shootBullet = false;
			bullet = 0;
		}
	}
	else {
		bullet = 0;
	}

	
	//left hand
	glPushMatrix();
	glTranslatef(0, 2, 0);	// move to centre on window
	{

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
		{
			glTranslatef(-1.85, 1.4, 0);
			glRotatef(leftArmRup, 1, 0, 0);
			glTranslatef(1.85, -1.4, 0);
			//white inner hand
			glPushMatrix();
			glTranslatef(-1.85, 1.4, 0);
			glRotatef(90, 1, 0, 0);
			renderTrapezoid(0.4, 0.2, 1.6);
			glPopMatrix();

			//Lower arm
			glPushMatrix();
			{
				glTranslatef(-1.85, 0.0, 0);
				glRotatef(leftArmRup1, 1, 0, 0);
				glRotatef(armRsword, -1, 0, 1);	// a
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

				glPushMatrix(); //palm push
				{
					glTranslatef(-1.85, -1.6, -0.1);
					if (boolHI) {
						if (leftArmRup1 <= 90) {
							glRotatef(leftArmRup1, 0, -1, 0);


						}
						else {
							glRotatef(90, 0, -1, 0);

						}
					}
					glTranslatef(1.85, 1.6, 0.1);

					if (boolWeapon == false){
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


						glPushMatrix();//push left finger
						{
							//rotate finger 1st part
							glTranslatef(-1.95, -2.6, -0.3);
							glRotatef(fingerRup, 0, 0, 1);
							glTranslatef(1.95, 2.6, 0.3);

							//1st part finger
							glPushMatrix();
							glTranslatef(-1.95, -2.8, -0.3);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -2.6, -0.3);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -2.8, -0.1);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -2.6, -0.1);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -2.8, 0.3);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -2.6, 0.3);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -2.8, 0.1);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -2.6, 0.1);
							renderSphere(0.1);
							glPopMatrix();

							//rotate finger 2nd part
							glTranslatef(-1.95, -3.0, -0.3);
							glRotatef(fingerRup, 0, 0, 1);
							glTranslatef(1.95, 3.0, 0.3);

							//2nd part
							glPushMatrix();
							glTranslatef(-1.95, -3.2, -0.3);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -3.0, -0.3);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -3.2, -0.1);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -3.0, -0.1);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -3.2, 0.3);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -3.0, 0.3);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -3.2, 0.1);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.95, -3.0, 0.1);
							renderSphere(0.1);
							glPopMatrix();

						}glPopMatrix();//finger push
						
						if (boolSword == true) {
							//sword
							//holder
							glPushMatrix();
							glTranslatef(-1.75, -2.2, -0.8);
							renderCylinder(0.2, 0.2, 1.6);
							glPopMatrix();

							//sword rear part
							glPushMatrix();
							glTranslatef(-1.75, -2.2, 0.8);
							renderTrapezoid(0.2, 0.4, 0.2);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.75, -2.2, 1.0);
							renderTrapezoid(0.4, 0.2, 0.4);
							glPopMatrix();
							//end sword rear part

							//sword front part
							glPushMatrix();
							glTranslatef(-1.75, -2.2, -1.4);
							renderTrapezoid(0.4, 0.8, 0.4);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.75, -2.2, -1.0);
							renderTrapezoid(0.8, 0.4, 0.4);
							glPopMatrix();

							//sword blade
							glPushMatrix();
							glTranslatef(-1.75, -2.1, -5.6);
							renderPrism(0.2, 4.2, 3);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(-1.75, -2.3, -5.6);
							glRotatef(60, 0, 0, 1);
							renderPrism(0.2, 4.2, 3);
							glPopMatrix();

							//sword sharp part
							glPushMatrix();
							glBegin(GL_TRIANGLES);
							glVertex3f(-1.75, -1.9, -5.6);
							glVertex3f(-1.75, -2.2, -6.0);
							glVertex3f(-1.6, -2.2, -5.6);

							glVertex3f(-1.75, -1.9, -5.6);
							glVertex3f(-1.75, -2.2, -6.0);
							glVertex3f(-1.9, -2.2, -5.6);

							glVertex3f(-1.75, -2.5, -5.6);
							glVertex3f(-1.75, -2.2, -6.0);
							glVertex3f(-1.6, -2.2, -5.6);

							glVertex3f(-1.75, -2.5, -5.6);
							glVertex3f(-1.75, -2.2, -6.0);
							glVertex3f(-1.9, -2.2, -5.6);

							glEnd();
							glPopMatrix();
							//end sword
						}
						}else {
						// Arm Weapons
						//gun
						glPushMatrix();
						glTranslatef(-1.85, -1.8, 0);
						glRotatef(90, 1, 0, 0);
						renderCylinder(0.3, 0.3, 0.7);
						glPopMatrix();

						//bullet
						glColor3f(1, 0, 0);
						glPushMatrix();
						glTranslatef(-1.85, -1.8 + bullet, 0);
						renderSphere(0.28);
						glPopMatrix();
						//end Right hand
					}//end else

				}
				glPopMatrix();//left palm pop
			}
			glPopMatrix(); //left lowerarm pop

		}
		glPopMatrix(); //left upperarm pop

	}
	glPopMatrix(); // translate up 2 via y-axis
}

void drawRightArm() {
	if (rightArmUpBool) {
		if (rightArmRup1 <= 50) {
			rightArmRup1 += armRSpeed;

		}
		else if (rightArmRup <= 15 || rightArmRup1 <= 110) {
			if (boolWeapon == false) {
				if (rightArmRup <= 15) {
					rightArmRup += armRSpeed / 2;
					rightArmRup1 += armRSpeed / 2;
				}
				else {
					rightArmRup1 += armRSpeed / 2;

				}
			}
			else {
				if (rightArmRup <= 15) {
					rightArmRup += armRSpeed / 2;
					if (rightArmRup1 <= 75)
						rightArmRup1 += armRSpeed / 2;
				}
				else {
					if (rightArmRup1 <= 75)
						rightArmRup1 += armRSpeed / 2;

				}
			}


		}
	}
	else if (rightArmDownBool) {
		if (rightArmRup1 >= 50) {
			rightArmRup1 -= armRSpeed;

		}
		else if (rightArmRup1 >= 0) {
			if (rightArmRup >= 0) {
				rightArmRup -= armRSpeed / 2;
				rightArmRup1 -= armRSpeed / 2;
			}
			else {
				rightArmRup1 -= armRSpeed;
			}


		}
	}
	//right hand
	glPushMatrix();
	glTranslatef(0, 2, 0);	// move to centre on window
	{
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

			glPushMatrix(); //push upper arm
			{
			glTranslatef(1.85, 1.4, 0);
			glRotatef(rightArmRup, 1, 0, 0);
			glTranslatef(-1.85, -1.4, 0);
			//white inner hand
			glPushMatrix();
			glTranslatef(1.85, 1.4, 0);
			glRotatef(90, 1, 0, 0);
			renderTrapezoid(0.4, 0.2, 1.6);
			glPopMatrix();

			//Lower arm
			glPushMatrix();
			{
				glTranslatef(-1.85, 0.0, 0);
				glRotatef(rightArmRup1, 1, 0, 0);
				glTranslatef(1.85, 0.0, 0);
				//black inner hand
				glPushMatrix();
				glTranslatef(1.85, 0.0, 0);
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

				glPushMatrix();
				{
					if (boolSword ==false && boolWeapon==false) {
						//palm
						glPushMatrix();//palm push
						{
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

							glPushMatrix();//push right finger

							glTranslatef(1.95, -2.6, -0.3);
							glRotatef(fingerRup1, 0, 0, -1);
							glTranslatef(-1.95, 2.6, 0.3);


							//1st part finger
							glPushMatrix();
							glTranslatef(1.95, -2.8, -0.3);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -2.6, -0.3);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -2.8, -0.1);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -2.6, -0.1);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -2.8, 0.3);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -2.6, 0.3);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -2.8, 0.1);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -2.6, 0.1);
							renderSphere(0.1);
							glPopMatrix();

							glTranslatef(1.95, -3.0, -0.3);
							glRotatef(fingerRup1, 0, 0, -1);
							glTranslatef(-1.95, 3.0, 0.3);

							//2nd part
							glPushMatrix();
							glTranslatef(1.95, -3.2, -0.3);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -3.0, -0.3);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -3.2, -0.1);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -3.0, -0.1);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -3.2, 0.3);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -3.0, 0.3);
							renderSphere(0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -3.2, 0.1);
							renderCubeWithoutGLU(0.1, 0.2, 0.1);
							glPopMatrix();

							glPushMatrix();
							glTranslatef(1.95, -3.0, 0.1);
							renderSphere(0.1);
							glPopMatrix();
						}glPopMatrix();

					}else{
							// Arm Weapons
							//gun
							glPushMatrix();
							glTranslatef(1.85, -1.8, 0);
							glRotatef(90, 1, 0, 0);
							renderCylinder(0.3, 0.3, 0.7);
							glPopMatrix();

							//bullet
							glColor3f(1, 0, 0);
							glPushMatrix();
							glTranslatef(1.85, -1.8+bullet, 0);
							renderSphere(0.28);
							glPopMatrix();
							//end Right hand
							
						
					}
				}
				glPopMatrix();//right finger pop
			}
			glPopMatrix();//right lower arm pop
		}
		glPopMatrix(); //right upper arm pop
	}
	glPopMatrix(); // translate up 2 via y-axis

}

void drawLeftLeg() {}
void drawRightLeg() {}

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

void robotStructure() {
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

void test1() {
	glPushMatrix();
	{
		glColor3f(0.0, 1.0, 1.0);
		glTranslatef(tX, tY, tZ);
		//glRotatef(tX, 0, 1, 0);	// test rotation, look cool mah
		//tX += tSpeed;			// press f to stop rotate, press s again to start rotate

		robotStructure();

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