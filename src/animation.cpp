#include "animation.h"
#include "state.h"

// Pure state mutations — no GL calls. Ported unchanged from Main.cpp lines 2470–2565.

void attack360() {
    if (rBodySpeed > 0.f) {
        rBodySpeed = 0.f;
    } else {
        rBodySpeed = 0.5f;
        boolWeapon = true;
        boolHI     = false;
        boolSword  = false;
        leftArmUpBool  = true;
        rightArmUpBool = true;
        leftArmRup  = raiseArmSpeed; leftArmRup1  = raiseArmSpeed;
        rightArmRup = raiseArmSpeed; rightArmRup1 = raiseArmSpeed;
    }
}

void walkFront() {
    // move left leg & (left arm back, right arm front)
    if (moveLeftLeg) {
        if (!leftLegAtFront) {
            if (handRightAngle <= 40.f && handRightAngle >= -40.f) {
                handRightAngle += wHandSpeed;
                handLeftAngle  -= wHandSpeed;
            }
            if (legLeftUpperAngle <= 40.f && legLeftUpperAngle >= -40.f) {
                legLeftUpperAngle += 1.f;
                legLeftLowerAngle -= 1.5f;
            } else {
                leftLegAtFront = true;
            }
        } else {
            if (handRightAngle != 0.f) {
                handRightAngle -= wHandSpeed;
                handLeftAngle  += wHandSpeed;
            }
            if (legLeftUpperAngle != 0.f) {
                legLeftUpperAngle -= 1.f;
                legLeftLowerAngle += 1.5f;
                tZ -= 0.01f;
            } else {
                leftLegAtFront = false;
                moveLeftLeg    = false;
                moveRightLeg   = true;
            }
        }
    } else {
        // move right leg & (right arm back, left arm front)
        if (!rightLegAtFront) {
            if (handLeftAngle <= 40.f && handLeftAngle >= -40.f) {
                handLeftAngle  += wHandSpeed;
                handRightAngle -= wHandSpeed;
            }
            if (leftRightUpperAngle <= 40.f && leftRightUpperAngle >= -40.f) {
                leftRightUpperAngle += 1.f;
                legRightLowerAngle  -= 1.5f;
            } else {
                rightLegAtFront = true;
            }
        } else {
            if (handLeftAngle != 0.f) {
                handLeftAngle  -= wHandSpeed;
                handRightAngle += wHandSpeed;
            }
            if (leftRightUpperAngle != 0.f) {
                leftRightUpperAngle -= 1.f;
                legRightLowerAngle  += 1.5f;
                tZ -= 0.01f;
            } else {
                rightLegAtFront = false;
                moveRightLeg    = false;
                moveLeftLeg     = true;
            }
        }
    }
}
