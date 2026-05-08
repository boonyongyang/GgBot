#include "input.h"
#include "state.h"
#include "animation.h"

// ── Key callback (replaces WM_KEYDOWN in WindowProcedure) ───────────────────
static void keyCallback(GLFWwindow* window, int key, int /*scancode*/,
                        int action, int /*mods*/) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    } else if (key == GLFW_KEY_1) { scane = 1;
    } else if (key == GLFW_KEY_2) { scane = 2;

    } else if (key == GLFW_KEY_3) {
        dif[0] = 0.f; dif[1] = 0.f;
        amb[0] = 1.f; amb[1] = 1.f;
        difM[1] = 0.f;
        materialFv = 1.f;
    } else if (key == GLFW_KEY_4) {
        dif[0] = 1.f; dif[1] = 1.f;
        amb[0] = 0.f; amb[1] = 0.f;
        difM[1] = 1.f;
        materialFv = 2.f;

    } else if (key == GLFW_KEY_SPACE) {
        // Full reset
        tX = 0.f; tY = 0.f; tZ = 0.f;
        mouseXRotate = 0.f; mouseYRotate = 0.f;
        mouseZRotate = 0.f; perspecZoomLevel = -2.f;
        ptX = 0.f; ptY = 0.f; prY = 0.f;
        hAngle = 0.f;
        rY = 0.f; rSpeed = 0.f;
        rBody = 0.f; rBodySpeed = 0.f;
        leftArmRup = 0.01f; leftArmRup1 = 0.01f;
        rightArmRup = 0.01f; rightArmRup1 = 0.01f;
        fingerRup = 0.01f; fingerRup1 = 0.01f;
        armRsword = 0.01f;
        leftArmUpBool = false; leftArmDownBool = false;
        rightArmUpBool = false; rightArmDownBool = false;
        fingerUpBool = false; fingerDownBool = false;
        armLeftBool = false; armRightBool = false;
        boolHI = false;
        handRightAngle = 0.f; handLeftAngle = 0.f;
        legLeftUpperAngle = 0.f; legLeftLowerAngle = 0.f;
        leftRightUpperAngle = 0.f; legRightLowerAngle = 0.f;
        leftLegAtFront = false; moveLeftLeg = false;
        rightLegAtFront = false; moveRightLeg = false;

    } else if (key == GLFW_KEY_UP) {
        if (isOrtho) { if (tZ > orthoNear) tZ += tSpeed; }
        else          { perspecZoomLevel += 1.f; }
    } else if (key == GLFW_KEY_DOWN) {
        if (isOrtho) { if (tZ < orthoFar) tZ -= tSpeed; }
        else          { perspecZoomLevel -= 1.f; }
    } else if (key == GLFW_KEY_LEFT) {
        if (tX > -10.f) tX -= tSpeed;
    } else if (key == GLFW_KEY_RIGHT) {
        if (tX <  10.f) tX += tSpeed;

    } else if (key == GLFW_KEY_A) { faceAngle = 270.f;
    } else if (key == GLFW_KEY_D) { faceAngle =  90.f;
    } else if (key == GLFW_KEY_W) { faceAngle =   0.f;
    } else if (key == GLFW_KEY_S) { faceAngle = 180.f;

    } else if (key == GLFW_KEY_5) {
        if (isOrtho) { if (ptX <  1.1f) ptX += ptSpeed; }
        else          { prY -= ptSpeed * 15.f; }
    } else if (key == GLFW_KEY_6) {
        if (isOrtho) { if (ptX > -1.1f) ptX -= ptSpeed; }
        else          { prY += ptSpeed * 15.f; }
    } else if (key == GLFW_KEY_7) {
        if (isOrtho) { if (ptY > -1.3f) ptY -= ptSpeed; }
        else          { perspecZoomLevel -= 1.f; }
    } else if (key == GLFW_KEY_8) {
        if (isOrtho) { if (ptY <  1.3f) ptY += ptSpeed; }
        else          { perspecZoomLevel += 1.f; }

    } else if (key == GLFW_KEY_P) {
        isOrtho = !isOrtho;
        tZ = 0.f;
        perspecZoomLevel = -2.f;

    } else if (key == GLFW_KEY_F) {
        rBodySpeed = (rBodySpeed > 0.f) ? 0.f : 0.5f;
    } else if (key == GLFW_KEY_R) {
        rSpeed = (rSpeed > 0.f) ? 0.f : 0.5f;

    // Left arm up (U) / down (I)
    } else if (key == GLFW_KEY_U) {
        leftArmDownBool = false;
        if (key == temp) { leftArmUpBool = false; temp = 0; }
        else             { leftArmUpBool = true;  temp = key; }
    } else if (key == GLFW_KEY_I) {
        leftArmUpBool = false;
        if (key == temp) { leftArmDownBool = false; temp = 0; }
        else             { leftArmDownBool = true;  temp = key; }

    // Right arm up (T) / down (Y)
    } else if (key == GLFW_KEY_T) {
        rightArmDownBool = false;
        if (key == temp) { rightArmUpBool = false; temp = 0; }
        else             { rightArmUpBool = true;  temp = key; }
    } else if (key == GLFW_KEY_Y) {
        rightArmUpBool = false;
        if (key == temp) { rightArmDownBool = false; temp = 0; }
        else             { rightArmDownBool = true;  temp = key; }

    // Fingers fist (Z) / open (X)
    } else if (key == GLFW_KEY_Z) {
        fingerDownBool = false;
        if (key == temp) { fingerUpBool = false; temp = 0; }
        else             { fingerUpBool = true;  temp = key; }
    } else if (key == GLFW_KEY_X) {
        fingerUpBool = false;
        if (key == temp) { fingerDownBool = false; temp = 0; }
        else             { fingerDownBool = true;  temp = key; }

    // Cannon shoot (C)
    } else if (key == GLFW_KEY_C) {
        if (key == temp) { shootBullet = false; temp = 0; }
        else             { shootBullet = true;  temp = GLFW_KEY_C; }

    // Sword attack (V)
    } else if (key == GLFW_KEY_V) {
        armRightBool = false;
        if (key == temp) { armLeftBool = false; temp = 0; }
        else             { armLeftBool = true;  temp = key; }

    // Weapon modes
    } else if (key == GLFW_KEY_F1) {
        boolWeapon = false; boolHI = false; boolSword = false;
    } else if (key == GLFW_KEY_F2) {
        boolWeapon = true; boolHI = false; boolSword = false;
        leftArmUpBool = true; rightArmUpBool = true;
        leftArmRup = raiseArmSpeed; leftArmRup1 = raiseArmSpeed;
        rightArmRup = raiseArmSpeed; rightArmRup1 = raiseArmSpeed;
    } else if (key == GLFW_KEY_F3) {
        boolWeapon = false; boolSword = true;
        fingerUpBool = true;
        fingerRup1 = 90.f; fingerRup = 90.f;
        rightArmRup = raiseArmSpeed; rightArmRup1 = raiseArmSpeed;
        leftArmUpBool = false; rightArmUpBool = false;
    } else if (key == GLFW_KEY_F4) {
        boolWeapon = false; boolHI = true;
        fingerUpBool = true;

    } else if (key == GLFW_KEY_Q) { walkFront();
    } else if (key == GLFW_KEY_E) { attack360();

    } else if (key == GLFW_KEY_L) { isLightOn   = !isLightOn;
    } else if (key == GLFW_KEY_G) { isAmbientOn = !isAmbientOn;
    } else if (key == GLFW_KEY_H) { isDiffuseOn = !isDiffuseOn;
    } else if (key == GLFW_KEY_J) { isSpecularOn = !isSpecularOn;

    } else if (key == GLFW_KEY_N) { hAngle += hSpeed;
    } else if (key == GLFW_KEY_B) { hAngle -= hSpeed;

    } else if (key == GLFW_KEY_K) {
        outerTextureNo = (outerTextureNo <= 3) ? outerTextureNo + 1 : 0;
    } else if (key == GLFW_KEY_M) {
        innerTextureNo = (innerTextureNo <= 1) ? innerTextureNo + 1 : 0;
    }
}

// ── Mouse button callback (replaces WM_LBUTTONDOWN) ─────────────────────────
static void mouseButtonCallback(GLFWwindow* window, int button,
                                 int action, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        mouseLastX = static_cast<float>(x);
        mouseLastY = static_cast<float>(y);
    }
}

// ── Cursor position callback (replaces WM_MOUSEMOVE + MK_LBUTTON) ───────────
static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        float x = static_cast<float>(xpos);
        float y = static_cast<float>(ypos);
        mouseYRotate += x - mouseLastX;
        mouseXRotate -= y - mouseLastY;
        mouseLastX = x;
        mouseLastY = y;
    }
}

// ── Scroll callback (replaces WM_MOUSEWHEEL) ────────────────────────────────
static void scrollCallback(GLFWwindow* /*window*/, double /*xoffset*/,
                            double yoffset) {
    // original: GET_WHEEL_DELTA_WPARAM / 150.0f  (WHEEL_DELTA=120 per notch)
    // GLFW yoffset is 1.0 per notch → scale to match
    perspecZoomLevel -= static_cast<float>(yoffset) * (120.f / 150.f);
}

void registerCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window,        keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window,  cursorPosCallback);
    glfwSetScrollCallback(window,     scrollCallback);
}
