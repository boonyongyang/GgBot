#include "state.h"

// ── Projection / camera ──────────────────────────────────────────────────────
int   scane    = 1;
float stacks   = 10.f;

float ptX = 0.f, ptY = 0.f, prY = 0.f;
float ptSpeed = 0.1f;

float tX = 0.f, tY = 0.f, tZ = 0.f, tSpeed = 0.f;
float rY = 0.f, rSpeed = 0.f;
float faceAngle = 0.f;

bool  isOrtho    = true;
float orthoNear  = -20.f, orthoFar  = 20.f;
float perspecNear = 10.f, perspecFar = 20.f;

// ── Mouse ────────────────────────────────────────────────────────────────────
float mouseLastX = 0.f, mouseLastY = 0.f;
float mouseXRotate = 0.f, mouseYRotate = 0.f, mouseZRotate = 0.f;
float perspecZoomLevel = -2.f;

// ── Head animation ───────────────────────────────────────────────────────────
float hx = 0.f, hy = 0.f, hz = 0.f, hAngle = 0.f, hSpeed = 1.f;

// ── Body rotation ────────────────────────────────────────────────────────────
float rBody = 0.f, rBodySpeed = 0.f;

// ── Neck ─────────────────────────────────────────────────────────────────────
float ny = 0.f;

// ── Arms ─────────────────────────────────────────────────────────────────────
bool  leftArmUpBool   = false, leftArmDownBool  = false;
bool  rightArmUpBool  = false, rightArmDownBool = false;
bool  fingerUpBool    = false, fingerDownBool   = false;
bool  armLeftBool     = false, armRightBool     = false;
bool  shootBullet     = false;
float armRSpeed       = 0.5f;
float leftArmRup      = 0.01f, leftArmRup1  = 0.01f;
float rightArmRup     = 0.01f, rightArmRup1 = 0.01f;
float fingerRup       = 0.01f, fingerRup1   = 0.01f;
float armRsword       = 0.01f;
int   temp            = 0;
float bullet          = 0.f;
float raiseArmSpeed   = 0.5f;
float handLeftAngle   = 0.f, handRightAngle = 0.f, wHandSpeed = 0.5f;

// ── Weapon / mode flags ──────────────────────────────────────────────────────
bool boolHI     = false;
bool boolWeapon = false;
bool boolSword  = false;

// ── Legs ─────────────────────────────────────────────────────────────────────
int   r = 1;
float legLeftUpperAngle  = 0.f, legLeftLowerAngle   = 0.f;
float leftRightUpperAngle = 0.f, legRightLowerAngle = 0.f;
float wLegSpeed = 1.f;
bool  leftLegAtFront  = false, moveLeftLeg  = true;
bool  rightLegAtFront = false, moveRightLeg = false;

// ── Lighting ─────────────────────────────────────────────────────────────────
bool  isLightOn   = true;
bool  isAmbientOn = true;
bool  isDiffuseOn = true;
bool  isSpecularOn = false;
float materialFv  = 1.f;
float lax = 0.f, lay = -1.f, laz = 0.f;
float ldx = 0.f, ldy =  3.f, ldz = 0.f;
float amb[4]  = { 1.f, 1.f, 1.f, 1.f };
float posA[3] = { 0.f,-1.f, 0.f };
float dif[4]  = { 0.f, 0.f, 1.f, 1.f };
float posD[3] = { 0.f, 3.f, 0.f };
float ambM[4] = { 1.f, 1.f, 1.f, 1.f };
float difM[4] = { 1.f, 0.f, 1.f, 1.f };

// ── Textures ─────────────────────────────────────────────────────────────────
GLuint textureArrInner[3] = {};
GLuint textureArrOuter[7] = {};
int    outerTextureNo = 0;
int    innerTextureNo = 0;
