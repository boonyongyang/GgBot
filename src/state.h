#pragma once
#include <glad/glad.h>

// ── Projection / camera ──────────────────────────────────────────────────────
extern int   scane;
extern float stacks;

extern float ptX, ptY, prY;
extern float ptSpeed;

extern float tX, tY, tZ, tSpeed;
extern float rY, rSpeed;
extern float faceAngle;

extern bool  isOrtho;
extern float orthoNear, orthoFar;
extern float perspecNear, perspecFar;

// ── Mouse ────────────────────────────────────────────────────────────────────
extern float mouseLastX, mouseLastY;
extern float mouseXRotate, mouseYRotate, mouseZRotate;
extern float perspecZoomLevel;

// ── Head animation ───────────────────────────────────────────────────────────
extern float hx, hy, hz, hAngle, hSpeed;

// ── Body rotation ────────────────────────────────────────────────────────────
extern float rBody, rBodySpeed;

// ── Neck ─────────────────────────────────────────────────────────────────────
extern float ny;

// ── Arms ─────────────────────────────────────────────────────────────────────
extern bool  leftArmUpBool, leftArmDownBool;
extern bool  rightArmUpBool, rightArmDownBool;
extern bool  fingerUpBool, fingerDownBool;
extern bool  armLeftBool, armRightBool;
extern bool  shootBullet;
extern float armRSpeed;
extern float leftArmRup, leftArmRup1;
extern float rightArmRup, rightArmRup1;
extern float fingerRup, fingerRup1;
extern float armRsword;
extern int   temp;        // last active key (was char, now int for GLFW key codes)
extern float bullet;
extern float raiseArmSpeed;
extern float handLeftAngle, handRightAngle, wHandSpeed;

// ── Weapon / mode flags ──────────────────────────────────────────────────────
extern bool boolHI;
extern bool boolWeapon;
extern bool boolSword;

// ── Legs ─────────────────────────────────────────────────────────────────────
extern int   r;
extern float legLeftUpperAngle, legLeftLowerAngle;
extern float leftRightUpperAngle, legRightLowerAngle;
extern float wLegSpeed;
extern bool  leftLegAtFront, moveLeftLeg;
extern bool  rightLegAtFront, moveRightLeg;

// ── Lighting ─────────────────────────────────────────────────────────────────
extern bool  isLightOn;
extern bool  isAmbientOn;
extern bool  isDiffuseOn;
extern bool  isSpecularOn;
extern float materialFv;
extern float lax, lay, laz;
extern float ldx, ldy, ldz;
extern float amb[4];
extern float posA[3];
extern float dif[4];
extern float posD[3];
extern float ambM[4];
extern float difM[4];

// ── Textures ─────────────────────────────────────────────────────────────────
extern GLuint textureArrInner[3];
extern GLuint textureArrOuter[7];
extern int    outerTextureNo;
extern int    innerTextureNo;
