#include <Windowsx.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#define WINDOW_TITLE "GgBot"

int scane = 1;
float stacks = 10;

// transformation for projection matrix
float ptX, ptY, prY = 0;
float ptSpeed = 0.1;

// transformation for object
float tX, tY, tZ = 0, tSpeed = 0.5;
float rY = 0, rSpeed = 0.5;
float faceAngle = 0;	// for WASD

// set Ortho View as default
boolean isOrtho = true;
float orthoNear = -20, orthoFar = 20.0;
float perspecNear = 10, perspecFar = 20;

// mouse movements
float mouseLastX = 0.0f, mouseLastY = 0.0f;
float mouseXRotate = 0.0f, mouseYRotate = 0.0f, mouseZRotate = 0.0f;
float perspecZoomLevel = -2.0f;

// other (Seperate to your own sections too)

// head animation
float hx = 0, hy = 0, hz = 0, hAngle = 0, hSpeed = 1;

// Body
float rBody = 0, rBodySpeed = 0;

// neck
float ny = 0;

// Hand
bool leftArmUpBool = false, leftArmDownBool = false;
bool rightArmUpBool = false, rightArmDownBool = false;
bool fingerUpBool = false, fingerDownBool = false;
bool armLeftBool = false, armRightBool = false;
bool shootBullet = false;
float armRSpeed = 0.1;
float	leftArmRup = 0.01, leftArmRup1 = 0.01,
rightArmRup = 0.01, rightArmRup1 = 0.01,
fingerRup = 0.01, fingerRup1 = 0.01,
armRsword = 0.01; //arm rotation variable, need reset for space
char temp = 'A';
float bullet = 0.0;
//extra feature
bool boolHI = false;//example
bool boolWeapon = false;
bool boolSword = false;

float raiseArmSpeed = 0.5;
float handLeftAngle, handRightAngle = 0, wHandSpeed = 0.5;	// for moving animation

// Leg
int r = 1;
float legLeftUpperAngle, legLeftLowerAngle = 0;
float leftRightUpperAngle, legRightLowerAngle = 0;
float wLegSpeed = 1;
bool leftLegAtFront = false, moveLeftLeg = true;
bool rightLegAtFront = false, moveRightLeg = false;

// Environment
float rCream = 0;

// Lighting
bool isLightOn = true;
bool isAmbientOn = true;
bool isDiffuseOn = true;
bool isSpecularOn = false;
float materialFv = 1;
float lax = 0, lay = -1, laz = 0;
float ldx = 0, ldy = 3, ldz = 0;
float amb[] = { 1.0, 1.0, 1.0 ,1.0 };			// red color ambient light 
// light0 position (0,6,0) above 
float posA[] = { lax, lay, laz };			// position of the light0 {x,y,z} - position 0,0,0 is actually inside the sphere 
float dif[] = { 0.0, 0.0, 1.0 ,1.0 };		// green color diffuse light
float posD[] = { ldx, ldy, ldz };		// position of the light1 {x,y,z} - position 0,0,0 is actually inside the sphere

float ambM[] = { 1.0, 1.0, 1.0 ,1.0 };		// red color ambient material
float difM[] = { 1.0, 0.0, 1.0 ,1.0 };		// blue color diffuse material

// Texture
GLuint textureArrInner[3];
GLuint textureArrOuter[7];
int outerTextureNo = 0;
int innerTextureNo = 0;

BITMAP BMP;				// bitmap structure
HBITMAP hBMP = NULL;	// bitmap handle

// function declarations
void walkFront();
void rotateBody();
void iceCream();	// delete ba no ice cream already

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
			scane = 1;
		}
		else if (wParam == 0x32) {
			scane = 2;
		}
		else if (wParam == 0x33) {
			dif[0] = 0.0, dif[1] = 0.0;
			amb[0] = 1.0, amb[1] = 1.0;
			difM[1] = 0.0;
			materialFv = 1;
		}
		else if (wParam == 0x34) {
			dif[0] = 1.0, dif[1] = 1.0;
			amb[0] = 0.0, amb[1] = 0.0;
			difM[1] = 1.0;
			materialFv = 2;
		}
		else if (wParam == VK_SPACE) {
			tX = 0, tY = 0, tZ = 0;
			mouseXRotate = 0.0f, mouseYRotate = 0.0f, mouseZRotate = 0.0f, perspecZoomLevel = -2.0f;
			ptX = 0, ptY = 0, prY = 0;

			// head
			hAngle = 0;

			// arm 
			leftArmRup = 0.01, leftArmRup1 = 0.01,
				rightArmRup = 0.01, rightArmRup1 = 0.01,
				fingerRup = 0.01, fingerRup1 = 0.01,
				armRsword = 0.01;
			leftArmUpBool = false, leftArmDownBool = false;
			rightArmUpBool = false, rightArmDownBool = false;
			fingerUpBool = false, fingerDownBool = false;
			armLeftBool = false, armRightBool = false;
			boolHI = false;

			handRightAngle = false, handLeftAngle = false;

			// leg angles
			legLeftUpperAngle = 0, legLeftLowerAngle = 0;
			leftRightUpperAngle = 0, legRightLowerAngle = 0;
			leftLegAtFront = false, moveLeftLeg = false;
			rightLegAtFront = false, moveRightLeg = false;

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
		//else if (wParam == 'O') {
		//	isOrtho = true;
		//	tZ = 0.0;
		//}
		else if (wParam == 'P') {
			isOrtho = !isOrtho;	// toggle perspective/ortho view
			tZ = 0.0;
		}
		else if (wParam == 'F') {
			if (rBodySpeed > 0) {
				rBodySpeed = 0.0;
			}
			else {
				rBodySpeed = 0.5;
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
		//else if (wParam == 'B') {	// not in used
		//	//finger Down
		//	armLeftBool = false;
		//	if (wParam == temp) {
		//		armRightBool = false;
		//		temp = NULL;
		//	}
		//	else {
		//		armRightBool = true;
		//		temp = wParam;
		//	}
		//}
		else if (wParam == VK_F1) {
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
			leftArmRup = raiseArmSpeed, leftArmRup1 = raiseArmSpeed,		// lift arm animation
				rightArmRup = raiseArmSpeed, rightArmRup1 = raiseArmSpeed;	// lift arm animation
			//rightArmRup = 15;
			//rightArmRup1 = 75;
			//leftArmRup = 15;
			//leftArmRup1 = 75;
		}
		else if (wParam == VK_F3) {
			boolWeapon = false;
			fingerUpBool = true;
			boolSword = true;
			fingerRup1 = 90;
			fingerRup = 90;
			//rightArmRup = 15;		// instant ready
			//rightArmRup1 = 75;	// instant ready
			rightArmRup = raiseArmSpeed, rightArmRup1 = raiseArmSpeed; // lift arm animation
		}
		else if (wParam == VK_F4) {
			boolWeapon = false;
			//boolSword
			boolHI = true;
			fingerUpBool = true;
		}
		else if (wParam == 'Q') {
			walkFront();
		}
		else if (wParam == 'E') {
			//walkBackwards();	// not sure what to do yet, maybe reverse walking
		}
		else if (wParam == 'L') {
			isLightOn = !isLightOn;
		}
		else if (wParam == 'N') {
			hAngle += hSpeed;
		}
		else if (wParam == 'B') {
			hAngle -= hSpeed;
		}
		else if (wParam == 'G') {
			isAmbientOn = !isAmbientOn;
		}
		else if (wParam == 'H') {
			isDiffuseOn = !isDiffuseOn;
		}
		else if (wParam == 'J') {
			isSpecularOn = !isSpecularOn;
		}
		else if (wParam == 'K') {
			if (outerTextureNo <= 3) {
				outerTextureNo++;
			}
			else {
				outerTextureNo = 0;
			}
		}
		else if (wParam == 'M') {
			if (innerTextureNo <= 1) {
				innerTextureNo++;
			}
			else {
				innerTextureNo = 0;
			}
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// ***************************************** PROJECTION *******************************************//

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
		//glEnable(GL_DEPTH_TEST);
	}
}

// ***************************************** LIGHTING *******************************************//

void lighting() {
	if (isLightOn) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_POSITION, posA);	// set the position of the light
	if (isAmbientOn) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}

	// Light 1: Green Color Diffuse Light at pos (6,0,0)
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_POSITION, posD);	// set the position of the light
	if (isDiffuseOn) {
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}

	glLightfv(GL_LIGHT2, GL_SPECULAR, dif);
	glLightfv(GL_LIGHT2, GL_POSITION, posD);	// set the position of the light
	if (isSpecularOn) {
		glEnable(GL_LIGHT2);
	}
	else {
		glDisable(GL_LIGHT2);
	}


}

// ***************************************** TEXTURES *******************************************//

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

void renderSphere(float r) {
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_SILHOUETTE);	// will change to GLU_FILL
	//gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, r, 50, 50);
	gluDeleteQuadric(sphere);
}

void renderDisk(float inR, float outR, float slices, float loops) {
	GLUquadricObj* obj = NULL;
	obj = gluNewQuadric();
	//gluQuadricDrawStyle(obj, GLU_LINE);	// will change to GLU_FILL		// GLU_LINE, GLU_SILHOUETTE, GLU_POINT
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricTexture(obj, true);
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
	//gluQuadricDrawStyle(obj, GLU_LINE);	// will change to GLU_FILL
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricTexture(obj, true);
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

void renderCuboid(float l, float h) {	// no use dao?
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

void renderSphereWithoutGLU(float radius)
{
	const float PI = 3.141592f;
	GLfloat x, y, z, sliceA, stackA;
	//GLfloat radius = 0.5;
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

		glTexCoord2f(0, 1);		glVertex3f(-x, -y, -z);
		glTexCoord2f(1, 1);		glVertex3f(-x, -y, z);
		glTexCoord2f(1, 0);		glVertex3f(x, -y, z);
		glTexCoord2f(0, 0);		glVertex3f(x, -y, -z);

		glTexCoord2f(0, 1);		glVertex3f(-x, -y, -z);
		glTexCoord2f(1, 1);		glVertex3f(-x, y, -z);
		glTexCoord2f(1, 0);		glVertex3f(x, y, -z);
		glTexCoord2f(0, 0);		glVertex3f(x, -y, -z);

		glTexCoord2f(0, 1);		glVertex3f(-x, -y, -z);
		glTexCoord2f(1, 1);		glVertex3f(-x, -y, z);
		glTexCoord2f(1, 0);		glVertex3f(-x, y, z);
		glTexCoord2f(0, 0);		glVertex3f(-x, y, -z);

		glTexCoord2f(0, 1);		glVertex3f(x, y, -z);
		glTexCoord2f(1, 1);		glVertex3f(x, y, z);
		glTexCoord2f(1, 0);		glVertex3f(-x, y, z);
		glTexCoord2f(0, 0);		glVertex3f(-x, y, -z);

		glTexCoord2f(0, 1);		glVertex3f(x, y, -z);
		glTexCoord2f(1, 1);		glVertex3f(x, y, z);
		glTexCoord2f(1, 0);		glVertex3f(x, -y, z);
		glTexCoord2f(0, 0);		glVertex3f(x, -y, -z);

		glTexCoord2f(0, 1);		glVertex3f(x, -y, z);
		glTexCoord2f(1, 1);		glVertex3f(x, y, z);
		glTexCoord2f(1, 0);		glVertex3f(-x, y, z);
		glTexCoord2f(0, 0);		glVertex3f(-x, -y, z);

		glEnd();
	}
	glPopMatrix();
}

void renderTrapezoidWithoutGLU(float top, float bot1, float bot2, float y, float z) {
	glPushMatrix();
	glTranslatef(-top / 2, -y / 2, -z / 2);
	//glColor3f(1.0, 1.0, 1.0);


	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);		glVertex3f(bot1, 0.0f, 0.0f);
	glTexCoord2f(1, 1);		glVertex3f(bot1, 0.0f, z);
	glTexCoord2f(1, 0);		glVertex3f(bot2, 0.0f, z);
	glTexCoord2f(0, 0);		glVertex3f(bot2, 0.0f, 0.0f);

	glTexCoord2f(0, 1);		glVertex3f(bot1, 0.0f, 0.0f);
	glTexCoord2f(1, 1);		glVertex3f(0.0f, y, 0.0f);
	glTexCoord2f(1, 0);		glVertex3f(top, y, 0.0f);
	glTexCoord2f(0, 0);		glVertex3f(bot2, 0.0f, 0.0f);

	glTexCoord2f(0, 1);		glVertex3f(bot1, 0.0f, 0.0f);
	glTexCoord2f(1, 1);		glVertex3f(bot1, 0.0f, z);
	glTexCoord2f(1, 0);		glVertex3f(0.0f, y, z);
	glTexCoord2f(0, 0);		glVertex3f(0.0f, y, 0.0f);

	glTexCoord2f(0, 1);		glVertex3f(top, y, 0.0f);
	glTexCoord2f(1, 1);		glVertex3f(top, y, z);
	glTexCoord2f(1, 0);		glVertex3f(0.0f, y, z);
	glTexCoord2f(0, 0);		glVertex3f(0.0f, y, 0.0f);

	glTexCoord2f(0, 1);		glVertex3f(top, y, 0.0f);
	glTexCoord2f(1, 1);		glVertex3f(top, y, z);
	glTexCoord2f(1, 0);		glVertex3f(bot2, 0.0f, z);
	glTexCoord2f(0, 0);		glVertex3f(bot2, 0.0f, 0.0f);

	glTexCoord2f(0, 1);		glVertex3f(bot2, 0.0f, z);
	glTexCoord2f(1, 1);		glVertex3f(top, y, z);
	glTexCoord2f(1, 0);		glVertex3f(0.0f, y, z);
	glTexCoord2f(0, 0);		glVertex3f(bot1, 0.0f, z);

	glEnd();

	glPopMatrix();

}

// ***************************************** HEAD & BODY *******************************************//

void robotStructure() {
	glPushMatrix();
	glTranslatef(0, 2.5, 0);	// move to centre on window
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
	}
	glPopMatrix();

	// leg spheres for reference

	renderSphere(0.1);	// centre sphere
	glPushMatrix();
	glTranslatef(0, -7, 0);
	renderSphere(0.1);	// bottom sphere middle
	glPopMatrix();
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
				glTranslatef(0.0, i * -0.2, 0.0);
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
			glTranslatef(-0.1, i * -0.2, 0.0);
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
			renderCubeWithoutGLU(0.3, 0.3, 0.2);
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
		glRotatef(ny += 201, 0.0, 1.0, 0.0);
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
		glRotatef(hAngle, 0.0, 1.0, 0.0);

		glPushMatrix();
		{
			glTranslatef(0.0, 4.85, 0.1);

			GLuint textureArrH[3];
			textureArrH[0] = loadTexture("textures/eyetest.bmp");
			drawEye();
			glDeleteTextures(1, &textureArrH[0]);

			textureArrH[1] = loadTexture("textures/metal2.bmp");
			drawNoseAndMouth();
			drawEar();
			glDeleteTextures(1, &textureArrH[1]);

			glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
			drawStructureHead();

		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawBody() {

	glPushMatrix();
	{

		glTranslatef(0.0, 1.0, 0.5);

		GLuint textureArr[3];
		textureArr[0] = loadTexture("textures/eyetest.bmp");
		drawHeart();
		glDeleteTextures(1, &textureArr[0]);

		//glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
		drawInnerBody();
		drawShoulder();
		drawNeck();

		//glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
		drawTopBack();
		drawRibs();
		drawPelvis();
		drawCore6Packs();
		//drawNeck();
		drawSpine();
		//drawInnerBodyStructure();
		drawChest();

	}
	glPopMatrix();


}

// ******************************************** ARM **********************************************//

void drawLeftArm() {
	if (leftArmUpBool) {
		if (leftArmRup1 <= 50) {
			leftArmRup1 += armRSpeed;

		}
		else if (leftArmRup <= 15 || leftArmRup1 <= 110) {
			if (boolWeapon == false || boolSword == true) {
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
		if (fingerRup1 <= 90 && fingerRup <= 90) {
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
		}
		else if (armRsword <= 60) {
			armRsword += armRSpeed * 2;
			leftArmRup1 -= armRSpeed * 2;
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
		bullet -= armRSpeed / 2;
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
	glTranslatef(-0.5, 2, 0);	// move to centre on window
	glRotatef(handLeftAngle, 1, 0, 0);
	{

		//white
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
		glTranslatef(-1.85, 1.8, 0);
		renderCubeWithoutGLU(0.8, 0.4, 0.80);
		glPopMatrix();

		//below white
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
		glTranslatef(-1.85, 1.2, -0.6);
		renderCubeWithoutGLU(0.8, 0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-1.85, 1.2, 0.6);
		renderCubeWithoutGLU(0.8, 0.2, 0.2);
		glPopMatrix();

		//black
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
		glTranslatef(-2.55, 1.8, 0);
		renderCubeWithoutGLU(0.3, 0.2, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-1.65, 1.8, 0);
		renderCubeWithoutGLU(0.6, 0.2, 1);
		glPopMatrix();

		//Connection
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
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
			glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
			glTranslatef(-1.85, 1.4, 0);
			glRotatef(90, 1, 0, 0);
			renderTrapezoid(0.4, 0.2, 1.6);
			glPopMatrix();

			//Lower arm
			glPushMatrix();
			{
				glTranslatef(-1.85, 0.0, 0);
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
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
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				glTranslatef(-1.85, -1.2, 0);
				glRotatef(90, 1, 0, 0);
				renderTrapezoid(0.6, 0.8, 0.2);
				glPopMatrix();

				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				glTranslatef(-1.85, -1.4, 0);
				glRotatef(90, 1, 0, 0);
				renderTrapezoid(0.8, 0.6, 0.6);
				glPopMatrix();
				//end black inner hand


				//yellow part
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
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

					if (boolWeapon == false) {
						//palm
						glPushMatrix();
						glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
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
							glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
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
							glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
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
					}
					else {
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
			if (boolWeapon == false && boolSword == false) {
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
	glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
	glTranslatef(0.5, 2, 0);	// move to centre on window
	glRotatef(handRightAngle, 1, 0, 0);
	{
		//white
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
		glTranslatef(1.85, 1.8, 0);
		renderCubeWithoutGLU(0.8, 0.4, 0.80);
		glPopMatrix();

		//below white
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
		glTranslatef(1.85, 1.2, -0.6);
		renderCubeWithoutGLU(0.8, 0.2, 0.2);
		glPopMatrix();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
		glTranslatef(1.85, 1.2, 0.6);
		renderCubeWithoutGLU(0.8, 0.2, 0.2);
		glPopMatrix();

		//black
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
		glTranslatef(2.55, 1.8, 0);
		renderCubeWithoutGLU(0.3, 0.2, 1);
		glPopMatrix();

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
		glTranslatef(1.65, 1.8, 0);
		renderCubeWithoutGLU(0.6, 0.2, 1);
		glPopMatrix();

		//Connection
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
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
			glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
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
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				glTranslatef(1.85, 0.0, 0);
				glRotatef(90, 1, 0, 0);
				renderTrapezoid(0.4, 0.6, 1.6);
				glPopMatrix();

				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				glTranslatef(1.85, -1.2, 0);
				glRotatef(90, 1, 0, 0);
				renderTrapezoid(0.6, 0.8, 0.2);
				glPopMatrix();

				glPushMatrix();

				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				glTranslatef(1.85, -1.4, 0);
				glRotatef(90, 1, 0, 0);
				renderTrapezoid(0.8, 0.6, 0.6);
				glPopMatrix();
				//end black inner hand

				//yellow part
				glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				glTranslatef(2, -0.6, 0);
				glRotatef(90, -1, 0.5, 0);
				renderTrapezoid(0.3, 0.02, 0.8);
				glPopMatrix();

				glPushMatrix();
				{

					if (boolSword == false && boolWeapon == false) {
						//palm
						glPushMatrix();//palm push
						{
							glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
							glTranslatef(2.05, -2.2, 0);
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

					}
					else {
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
						glTranslatef(1.85, -1.8 + bullet, 0);
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

// ******************************************** LEG **********************************************//

void walkFront() {

	// move left leg & (left arm back, right arm front)
	if (moveLeftLeg) {

		if (!leftLegAtFront) {

			if (handRightAngle <= 40 && handRightAngle >= -40) {	// right hand moving up
				handRightAngle += wHandSpeed;
				handLeftAngle -= wHandSpeed;
			}

			if (legLeftUpperAngle <= 40 && legLeftUpperAngle >= -40) {
				legLeftUpperAngle += 1;
				legLeftLowerAngle -= 1.5;
			}
			else {
				leftLegAtFront = true;
			}
		}
		else {

			if (!handRightAngle == 0) {		// right hand moving down
				handRightAngle -= wHandSpeed;
				handLeftAngle += wHandSpeed;
			}

			if (!legLeftUpperAngle == 0) {
				legLeftUpperAngle -= 1;
				legLeftLowerAngle += 1.5;
				tZ -= 0.01;
			}
			else {
				leftLegAtFront = false;
				moveLeftLeg = false;
				moveRightLeg = true;
			}
		}
	}
	// move right leg & (right arm back, left arm front)
	else {

		if (!rightLegAtFront) {

			if (handLeftAngle <= 40 && handLeftAngle >= -40) {	// left hand moving up
				handLeftAngle += wHandSpeed;
				handRightAngle -= wHandSpeed;
			}

			if (leftRightUpperAngle <= 40 && leftRightUpperAngle >= -40) {
				leftRightUpperAngle += 1;
				legRightLowerAngle -= 1.5;
			}
			else {
				rightLegAtFront = true;
			}
		}
		else {

			if (!handLeftAngle == 0) {		// left hand moving down
				handLeftAngle -= wHandSpeed;
				handRightAngle += wHandSpeed;
			}

			if (!leftRightUpperAngle == 0) {
				leftRightUpperAngle -= 1;
				legRightLowerAngle += 1.5;
				tZ -= 0.01;
			}
			else {
				rightLegAtFront = false;
				moveRightLeg = false;
				moveLeftLeg = true;
			}
		}
	}

}

void walkBackwards() {

	// move left leg & (left arm back, right arm front)
	if (moveLeftLeg) {

		if (!leftLegAtFront) {

			if (handRightAngle <= 40 && handRightAngle >= -40) {	// right hand moving up
				handRightAngle -= wHandSpeed;
				handLeftAngle += wHandSpeed;
			}

			if (legLeftUpperAngle <= 40 && legLeftUpperAngle >= -40) {
				legLeftUpperAngle -= 1;
				legLeftLowerAngle += 1.5;
			}
			else {
				leftLegAtFront = true;
			}
		}
		else {

			if (!handRightAngle == 0) {		// right hand moving down
				handRightAngle += wHandSpeed;
				handLeftAngle -= wHandSpeed;
			}

			if (!legLeftUpperAngle == 0) {
				legLeftUpperAngle += 1;
				legLeftLowerAngle -= 1.5;
				tZ += 0.01;
			}
			else {
				leftLegAtFront = false;
				moveLeftLeg = false;
				moveRightLeg = true;
			}
		}
	}
	// move right leg & (right arm back, left arm front)
	else {

		if (!rightLegAtFront) {

			if (handLeftAngle <= 40 && handLeftAngle >= -40) {	// left hand moving up
				handLeftAngle -= wHandSpeed;
				handRightAngle += wHandSpeed;
			}

			if (leftRightUpperAngle <= 40 && leftRightUpperAngle >= -40) {
				leftRightUpperAngle -= 1;
				legRightLowerAngle += 1.5;
			}
			else {
				rightLegAtFront = true;
			}
		}
		else {

			if (!handLeftAngle == 0) {		// left hand moving down
				handLeftAngle += wHandSpeed;
				handRightAngle -= wHandSpeed;
			}

			if (!leftRightUpperAngle == 0) {
				leftRightUpperAngle += 1;
				legRightLowerAngle -= 1.5;
				tZ += 0.01;
			}
			else {
				rightLegAtFront = false;
				moveRightLeg = false;
				moveLeftLeg = true;
			}
		}
	}

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
		//glColor3f(1, 0, 0);
		//renderCylinder(1.5 * r, 1.5 * r, 5 * r);
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
			{
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
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLegUpperArmor(float d) {

	glPushMatrix();
	{
		glScalef(0.3, 0.3, 0.3);

		// upper armor plates
		glPushMatrix();
		{
			glTranslatef(0, 0, -2);
			glRotatef(-90, 1, 0, 0);
			glScalef(0.8, 1, 0.8);

			renderTrapezoidWithoutGLU(1.5, -0.25, 1.75, 0.5, 9);

			glPushMatrix();
			glTranslatef(2, -1, 0);
			glRotatef(-45, 0, 0, 1);
			glScalef(0.8, 0.9, 0.8);
			renderTrapezoidWithoutGLU(1.5, -0.25, 1.75, 0.5, 11);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-2, -1, 0);
			glRotatef(45, 0, 0, 1);
			glScalef(0.8, 0.9, 0.8);
			renderTrapezoidWithoutGLU(1.5, -0.25, 1.75, 0.5, 11);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1, -1, -1);
			glRotatef(-45, 0, 0, 1);
			glScalef(0.7, 1, 0.8);
			renderTrapezoidWithoutGLU(1.5, -0.25, 1.75, 0.5, 11);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-1, -1, -1);
			glRotatef(45, 0, 0, 1);
			glScalef(0.7, 1, 0.8);
			renderTrapezoidWithoutGLU(1.5, -0.25, 1.75, 0.5, 11);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.5, -2, -1);
			glRotatef(-45, 0, 0, 1);
			glScalef(0.7, 1, 0.8);
			renderTrapezoidWithoutGLU(1.5, -0.25, 1.75, 0.5, 11);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-1.5, -2, -1);
			glRotatef(45, 0, 0, 1);
			glScalef(0.7, 1, 0.8);
			renderTrapezoidWithoutGLU(1.5, -0.25, 1.75, 0.5, 11);
			glPopMatrix();
		}
		glPopMatrix();

		// big side plate
		glPushMatrix();
		{
			glTranslatef(d * 2.5, 0, 0);
			glRotatef(-90, 0, 1, 0);
			glRotatef(-15, 0, 0, 1);
			glPushMatrix();
			glRotatef(-d * 90, 1, 0, 0);	// -ve is right side plate, +ve is left side plate
			renderTrapezoidWithoutGLU(1.5, -0.25, 1.75, 0.5, 7);
			glPopMatrix();
		}
		glPopMatrix();

		// cylinders behind thigh 1
		glPushMatrix();
		{
			glTranslatef(0, -4, 1.3);

			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			renderCylinder(0.3, 0.3, 7.5);
			glPopMatrix();
		}
		glPopMatrix();

		// cylinders behind thigh 2
		glPushMatrix();
		{
			glTranslatef(-0.5, -4, 1.3);

			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			renderCylinder(0.3, 0.3, 7.5);
			glPopMatrix();
		}
		glPopMatrix();

		// cylinders behind thigh 3
		glPushMatrix();
		{
			glTranslatef(0.5, -4, 1.3);

			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			renderCylinder(0.3, 0.3, 7.5);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLegLowerArmor(float n) {
	float r = 0.2;
	glPushMatrix();
	{
		glScalef(0.3, 0.3, 0.3);
		glPushMatrix();
		{
			if (n <= 4) {
				glTranslatef(1.5, 0, 1.5);
			}
			if (n >= 8) {
				glTranslatef(0.5, 0, 1.2);
				//glTranslatef(1, 0, 2.4);
			}
			//glRotatef(90, 1, 0, 0);

			for (int i = 0; i <= n; i++) {
				glRotatef(360 / n, 0, 1, 0);
				glTranslatef(1, 0, 0);
				//glTranslatef(20 / n, 0, 0);
				//glTranslatef(1 / (2 * n), 0, 1 / (2 * n));
				//r += 0.1;
				glColor3f(0.2, 0.3, r);
				renderCubeWithoutGLU(1, 4, 0.2);
			}
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

void drawLegFoot(float d) {
	glPushMatrix();
	{
		glRotatef(90, 1, 0, 0);
		glRotatef(180, 0, 0, 1);
		glScalef(0.5, 0.5, 0.5);

		renderCubeWithoutGLU(0.75, 0.5, 0.25);

		glPushMatrix();
		glTranslatef(0, 0, -1.75);
		renderCylinder(0.2, 0.2, 2);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 0, -0.5);
		renderCubeWithoutGLU(0.75, 0.5, 0.25);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 1, 0);
		renderCubeWithoutGLU(0.75, 0.5, 0.25);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 2, 0);
		renderTrapezoidWithoutGLU(1, -0.25, 1.25, 1, 0.5);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 1, -0.5);
		renderTrapezoidWithoutGLU(1, -0.25, 1.25, 1, 0.5);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 0, -1);
		renderTrapezoidWithoutGLU(1, -0.25, 1.25, 1, 0.5);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, -1, 0);
		glRotatef(180, 0, 0, 1);
		renderTrapezoidWithoutGLU(1, -0.25, 1.25, 1, 0.5);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-1, 0, 0);
		glRotatef(90, 0, 0, 1);
		renderTrapezoidWithoutGLU(1, -0.25, 1.25, 0.5, 0.5);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1, 0, 0);
		glRotatef(270, 0, 0, 1);
		renderTrapezoidWithoutGLU(1, -0.25, 1.25, 0.5, 0.5);
		glPopMatrix();


		//glPushMatrix();
		//glTranslatef(1.25, 0, -0.25);
		//glRotatef(270, 0, 0, 1);
		//renderSphereWithoutGLU(0.5);
		//glPopMatrix();

		glPushMatrix();
		glTranslatef(d * 1.25, 0, -0.25);
		glRotatef(-d * 90, 0, 0, 1);
		renderSphereWithoutGLU(0.5);
		glPopMatrix();

		//glPushMatrix();
		//glTranslatef(0, 2, -0.25);
		//glRotatef(-90, 1, 0, 0);
		//renderSphereWithoutGLU(0.5);
		//glPopMatrix();
	}
	glPopMatrix();
}

void drawLeftLeg() {

	// whole leg
	glPushMatrix();
	{
		glRotatef(legLeftUpperAngle, 1, 0, 0);

		// upper leg section
		glPushMatrix();
		{
			//glRotatef(legLeftAngle2, 1, 0, 0);

			// left leg upper nerve (thigh)
			glPushMatrix();
			{
				glTranslatef(-0.65, -2, 0);	// move to left leg position
				glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
				drawLegInner(10);
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				drawLegUpperArmor(-1);

			}
			glPopMatrix();
		}
		glPopMatrix();

		// lower leg section
		glPushMatrix();
		{
			glTranslatef(0, -3.4, 0);
			glRotatef(legLeftLowerAngle, 1, 0, 0);
			glTranslatef(0, 3.4, 0);

			// left knee leg joint
			glPushMatrix();
			{
				glTranslatef(-0.39, -3.8, -0.3);	// knee with 3 joints
				glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
				drawLegKnee();
				//drawLegKneeArmor();
			}
			glPopMatrix();


			// left leg lower nerve (shin)
			glPushMatrix();
			{
				glTranslatef(-0.65, -5, 0);	// move to left leg position
				//glRotatef(legLeftLowerAngle, 1, 0, 0);
				glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
				drawLegInner(7);
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				drawLegLowerArmor(8);
			}
			glPopMatrix();

			// left leg joint (ankle)
			glPushMatrix();
			{
				glTranslatef(-0.45, -6.3, -0.25);	// move to left leg position
				//glRotatef(legLeftLowerAngle, 1, 0, 0);
				glScalef(0.8, 0.8, 0.8);
				glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
				drawLegKnee();
			}
			glPopMatrix();

			// left foot
			glPushMatrix();
			{
				glTranslatef(-0.65, -6.8, 0);	// move to left leg position
				//glRotatef(legLeftLowerAngle, 1, 0, 0);
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				drawLegFoot(1);
			}
			glPopMatrix();

			//glRotatef(legLeftLowerAngle, 1, 0, 0);
		}
		glPopMatrix();


	}
	glPopMatrix();

}

void drawRightLeg() {

	// whole leg
	glPushMatrix();
	{
		glRotatef(leftRightUpperAngle, 1, 0, 0);

		// right leg upper nerve (thigh)
		glPushMatrix();
		{
			glTranslatef(0.65, -2, 0);	// move to right leg position

			glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);

			drawLegInner(10);


			glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
			drawLegUpperArmor(1);

		}
		glPopMatrix();

		// lower leg section
		glPushMatrix();
		{
			glTranslatef(0, -3.4, 0);
			glRotatef(legRightLowerAngle, 1, 0, 0);
			glTranslatef(0, 3.4, 0);

			// right knee leg joint
			glPushMatrix();
			{
				glTranslatef(0.91, -3.8, -0.3);	// knee with 3 joints

				glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
				drawLegKnee();

			}
			glPopMatrix();

			// right leg lower nerve (shin)
			glPushMatrix();
			{
				glTranslatef(0.65, -5, 0);	// move to right leg position

				glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
				drawLegInner(7);
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				drawLegLowerArmor(8);

			}
			glPopMatrix();

			// right leg joint (ankle)
			glPushMatrix();
			{
				glTranslatef(0.85, -6.3, -0.25);	// move to right leg position
				glScalef(0.8, 0.8, 0.8);
				glBindTexture(GL_TEXTURE_2D, textureArrInner[innerTextureNo]);
				drawLegKnee();
			}
			glPopMatrix();

			// right foot
			glPushMatrix();
			{
				glTranslatef(0.65, -6.8, 0);	// move to right leg position
				glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
				drawLegFoot(-1);
			}
			glPopMatrix();
		}
		glPopMatrix();
		//glDeleteTextures(1, &textureArr[0]);

		//glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();


}

// ******************************************** ENVIRONMENT **********************************************//

void drawOcean() {
	glPushMatrix();

	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 6);

	GLuint textureArr[3];
	glDeleteTextures(1, &textureArr[0]);
	textureArr[1] = loadTexture("textures/ocean.bmp");
	// add here la

	renderDisk(0, 20, 50, 50);
	//renderSphere(100);
	glDeleteTextures(1, &textureArr[3]);

	glPopMatrix();
}


// ******************************************** DISPLAY **********************************************//

void summonGgBot() {
	glPushMatrix();
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambM);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);
		glMaterialfv(GL_FRONT, GL_SPECULAR, difM);

		glTranslatef(tX, tY, tZ);
		//glRotatef(bodyAngle, 1, 0, 0);
		//moveX++;

		GLuint textureArr[3];

		drawHead();
		drawBody();

		//GLuint textureArr[3];
		//textureArrOuter[outerTextureNo];
		//glBindTexture(GL_TEXTURE_2D, textureArrOuter[outerTextureNo]);
		drawRightArm();
		drawLeftArm();

		drawLeftLeg();
		drawRightLeg();



	}
	glPopMatrix();
}

void scene1() {
	glPushMatrix();
	{
		glColor3f(0.0, 1.0, 1.0);
		//glTranslatef(tX, tY, tZ);
		//glRotatef(faceAngle, 0.0f, 1.0f, 0.0f);
		//glRotatef(tX, 0, 1, 0);		// test rotation, look cool mah
		//tX += tSpeed;					// press s to stop rotate, press f again to start rotate

		//glTranslatef(tX, tY, tZ);           
		//robotStructure();

		glColor3f(1.0, 1.0, 0.0);
		glTranslatef(0, 0.8, 0);
		//glMaterialfv(GL_FRONT, GL_AMBIENT, ambM1);	// different material can have different material
		summonGgBot();
	}
	glPopMatrix();
}

void scene2() {
	glColor3f(1.0, 0.0, 1.0);

	glPushMatrix();

	drawOcean();
	glPushMatrix();
	{
		glTranslatef(0, 3, -0.5);
		glRotatef(rCream, 0, 1, 0);
		rCream++;

		glPushMatrix();
		iceCream();
		glTranslatef(1, 0, 0);
		iceCream();
		glTranslatef(0, 0, 1);
		iceCream();
		glTranslatef(-1, 0, 0);
		glScalef(0.5, 0.5, 0.5);
		iceCream();
		glPopMatrix();
	}

	glPopMatrix();


	glPushMatrix();
	{
		glTranslatef(3, 0, 8);
		glScalef(0.8, 0.8, 0.8);
		summonGgBot();
	}
	glPopMatrix();


	glPopMatrix();
}

void scene3() {
	glColor3f(1.0, 0.0, 1.0);

	glPushMatrix();
	robotStructure();

	//drawRightArm();
	//drawLeftArm();

	//drawLeftLeg();
	//drawRightLeg();

	glPopMatrix();
}

void scene4() {
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





void display()
{
	// project initialization
	{
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
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
	}
	glRotatef(faceAngle, 0.0f, 1.0f, 0.0f);

	// texture for outer
	textureArrOuter[0] = loadTexture("textures/metal2.bmp");
	textureArrOuter[1] = loadTexture("textures/camo128.bmp");
	textureArrOuter[2] = loadTexture("textures/wood.bmp");
	textureArrOuter[3] = loadTexture("textures/camoyellow128.bmp");
	textureArrOuter[4] = loadTexture("textures/camoblue128.bmp");

	// texture for inner
	textureArrInner[0] = loadTexture("textures/darksteel32.bmp");
	textureArrInner[1] = loadTexture("textures/bronzepattern.bmp");
	textureArrInner[2] = loadTexture("textures/armorseamless.bmp");

	switch (scane) {
	case 1:
		scene1();	// main robot structure here for ref
		break;
	case 2:
		scene2();	// change to what u want for test
		break;
	case 3:
		scene3();	// change to what u want for test
		break;
	case 4:
		scene4();	// change to what u want for test
		break;
	default:
		scene1();
		break;
	}
	glDeleteTextures(1, &textureArrInner[0]);
	glDeleteTextures(1, &textureArrOuter[0]);
	glDisable(GL_TEXTURE_2D);


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
void iceCream() {
	//glScalef(0.1,0.1,0.1);
	glRotatef(0.2, 0.0, 1.0, 0.1);	// rotate the whole ice cream
	// ICE CREAM CONE
	glPushMatrix();
	glColor3f(0.6, 0.4, 0.2);
	glTranslatef(0.0, -0.95, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	renderCylinder(0.0, 0.3, 0.6);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(0.0, -0.95, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	renderCylinder(0.0, 0.3, 0.6);
	glPopMatrix();

	// FIRST SCOOP
	glPushMatrix();
	glTranslatef(0.0, -0.2, 0.0);
	glColor3f(0.88, 0.58, 0.62);	// rgb(225, 147, 157)
	renderSphere(0.33);
	glPopMatrix();

	// SECOND SCOOP
	glPushMatrix();
	glTranslatef(0.0, 0.3, 0.0);
	glColor3f(0.57, 0.35, 0.28);		// rgb(145, 89, 72)
	renderSphere(0.33);
	glPopMatrix();

	// LOVE LETTER BISCUIT
	glPushMatrix();
	glTranslatef(-0.1, 0.45, 0.0);
	glRotatef(-75, 1.0, 0.0, 0.0);
	glColor3f(0.4, 0.21, 0.11);			// rgb(129, 56, 29)
	renderCylinder(0.05, 0.05, 0.4);
	glPopMatrix();

	// CHEERY
	glPushMatrix();
	glTranslatef(0.1, 0.65, 0.0);
	glColor3f(0.8, 0.0, 0.2);
	renderSphere(0.1);
	glPopMatrix();

	// OREO BISCUIT
	glPushMatrix();		// black biscuit
	glTranslatef(0.0, 0.5, 0.3);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glColor3f(0, 0, 0);
	renderCylinder(0.18, 0.18, 0.02);
	glPopMatrix();

	glPushMatrix();		// white filling
	glTranslatef(0.025, 0.5, 0.3);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glColor3f(1, 1, 1);
	renderCylinder(0.18, 0.18, 0.02);
	glPopMatrix();

	glPushMatrix();		// black biscuit
	glTranslatef(0.05, 0.5, 0.3);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glColor3f(0, 0, 0);
	renderCylinder(0.18, 0.18, 0.02);
	glPopMatrix();

	// SPRINKLES 
	glPushMatrix();
	glTranslatef(0.25, 0.52, 0.03);
	glRotatef(-60, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0); // yellow
	renderCylinder(0.005, 0.005, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.52, 0.05);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0); // white
	renderCylinder(0.005, 0.005, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.52, 0.04);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glColor3f(1.0, 0.0, 1.0); // purple
	renderCylinder(0.005, 0.005, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.52, 0.03);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 1.0); // green
	renderCylinder(0.005, 0.005, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, 0.52, 0.03);
	glRotatef(60, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0); // yellow
	renderCylinder(0.005, 0.005, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, 0.52, 0.05);
	glRotatef(70, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0); // white
	renderCylinder(0.005, 0.005, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, 0.52, 0.04);
	glRotatef(80, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glColor3f(1.0, 0.0, 1.0); // purple
	renderCylinder(0.005, 0.005, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, 0.52, 0.03);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glColor3f(0.0, 1.0, 1.0); // green
	renderCylinder(0.005, 0.005, 0.05);
	glPopMatrix();
}
