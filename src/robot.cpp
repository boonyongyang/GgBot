#include "robot.h"
#include "mesh.h"
#include "state.h"
#include "texture.h"
#include <glm/gtc/matrix_transform.hpp>

// ── Per-frame draw context ────────────────────────────────────────────────────
struct DrawCtx {
    GLuint prog;
    Uniforms u;
    GLuint outer, inner;
    DrawCtx() {
        glm::mat4 v, p;
        getFrameContext(prog, u, v, p);
        outer = textureArrOuter[outerTextureNo];
        inner = textureArrInner[innerTextureNo];
    }
};

// ── Transform shorthands ──────────────────────────────────────────────────────
static glm::mat4 T(float x, float y, float z) {
    return glm::translate(glm::mat4(1.f), {x, y, z});
}
static glm::mat4 R(float deg, float x, float y, float z) {
    return glm::rotate(glm::mat4(1.f), glm::radians(deg), {x, y, z});
}
static glm::mat4 Sc(float x, float y, float z) {
    return glm::scale(glm::mat4(1.f), {x, y, z});
}

// ─────────────────────────────────────────────────────────────────────────────
void drawSpineJoint(glm::mat4 m) {
    DrawCtx c;
    renderSphere(c.prog,c.u, m,                   c.outer,{1,1,0}, 0.1f);
    renderSphere(c.prog,c.u, m*T( 0.15f,0,0),     0,      {1,0,0}, 0.05f);
    renderSphere(c.prog,c.u, m*T(-0.15f,0,0),     0,      {1,0,0}, 0.05f);
    renderTrapezoidGLU(c.prog,c.u, m*T( 0.2f,0,0)*R( 90,0,1,0), 0,{0,1,1}, 0.05f,0.025f,0.2f);
    renderTrapezoidGLU(c.prog,c.u, m*T(-0.2f,0,0)*R(-90,0,1,0), 0,{0,1,1}, 0.05f,0.025f,0.2f);
}

void drawScale(glm::mat4 m) {
    DrawCtx c;
    for (int i = 0; i <= 4; ++i)
        renderTrapezoid(c.prog,c.u, m*T(0,i*-0.2f,0)*R(180,1,0,0),
                        0,{1,0,1}, 0.2f,0.f,0.f,0.2f,0.4f);
}

void drawEye(glm::mat4 m) {
    DrawCtx c;
    renderCube(c.prog,c.u, m*T(0,0.2f,-0.32f)*R(30,1,0,0),
               texEye,{1,1,1}, 0.3f,0.1f,0.2f);
}

void drawNoseAndMouth(glm::mat4 m) {
    DrawCtx c;
    renderTrapezoid(c.prog,c.u, m*T(0,-0.25f,-0.6f)*R(90,1,0,0),
                    texMetal,{0,1,1}, 1.1f,0.2f,0.9f,0.3f,0.3f);
    renderTrapezoid(c.prog,c.u,
                    m*T(0,0.24f,-0.52f)*R(15,1,0,0)*R(90,0,1,0)*R(90,0,0,1),
                    texMetal,{1,1,0}, 0.6f,-0.1f,-0.1f,0.3f,0.7f);
}

void drawEar(glm::mat4 m) {
    DrawCtx c;
    renderCylinder(c.prog,c.u, m*T(-0.63f,0,0)*R(90,0,1,0),
                   c.outer,{1,1,1}, 0.15f,0.15f,1.3f);
    renderPrism(c.prog,c.u, m*Sc(1,1,0.5f)*T(-0.6f,0.1f,0)*R(90,0,1,0),
                c.inner,{0,0,1}, 0.7f,0.05f,3);
    renderPrism(c.prog,c.u, m*Sc(1,1,0.5f)*T(0.6f,0.1f,0)*R(90,0,1,0),
                c.inner,{0,0,1}, 0.7f,0.05f,3);
    renderCylinder(c.prog,c.u, m*Sc(1,0.5f,0.5f)*T(-0.6f,-0.5f,0)*R(90,0,1,0),
                   c.inner,{1,1,1}, 0.35f,0.35f,1.2f);
}

void drawHat(glm::mat4 m) {
    DrawCtx c;
    renderCone(c.prog,c.u,  m*T(0,1.5f,0)*R(90,1,0,0), c.inner,{1,1,1}, 0.3f,1.f);
    renderDisk(c.prog,c.u,  m*T(0,0.5f,0)*R(90,1,0,0), c.inner,{1,1,1}, 0.f,0.5f,50,50);
}

void drawStructureHead(glm::mat4 m) {
    DrawCtx c;
    renderTrapezoid(c.prog,c.u, m*R(90,0,1,0)*R(180,1,0,0),
                    c.outer,{1,0,0}, 1.1f,0.2f,0.9f,0.9f,1.1f);
}

void drawHead(glm::mat4 m) {
    glm::mat4 base = m * R(hAngle,0,1,0) * T(0,4.85f,0.1f);
    drawEye(base);
    drawNoseAndMouth(base);
    drawEar(base);
    drawStructureHead(base);
    drawHat(base);
}

void drawLegInnerNerve(glm::mat4 m, float rv, float h) {
    DrawCtx c;
    renderCylinder(c.prog,c.u, m*T(0,h/2,0)*R(90,1,0,0), 0,{1,1,0}, rv,rv,h);
    renderPrism(c.prog,c.u,    m*T(0,(0.8f*h)/2,0)*R(90,1,0,0), 0,{1,0,0}, 2.5f*rv,0.8f*h,8);
    renderPrism(c.prog,c.u,    m*T(0, h/2,-0.4f),  0,{0,1,1}, 3.f*rv,4.f*rv,5);
    renderPrism(c.prog,c.u,    m*T(0,-h/2,-0.4f)*R(180,0,0,1), 0,{0,1,1}, 3.f*rv,4.f*rv,5);
}

void drawLegInner(glm::mat4 m, float h) {
    glm::mat4 base = m * Sc(0.3f,0.3f,0.3f) * R(90,0,1,0) * T(-0.5f,0,-0.5f);
    drawLegInnerNerve(base,             0.2f, h);
    drawLegInnerNerve(base*T(1,0,0),    0.2f, h);
    drawLegInnerNerve(base*T(0,0,1),    0.2f, h);
    drawLegInnerNerve(base*T(1,0,1),    0.2f, h);
}

void drawCoreDetail1(glm::mat4 m) {
    DrawCtx c;
    for (int i = 0; i < 6; ++i) {
        renderTrapezoid(c.prog,c.u, m*T(-0.1f,i*-0.2f,0)*R(-10,1,0,0)*R(90,0,0,1),
                        0,{1,0,0}, 0.4f,-0.1f,0.3f,0.2f,0.1f);
        renderTrapezoid(c.prog,c.u, m*T(0.1f,i*-0.2f,0)*R(-10,1,0,0)*R(180,0,1,0)*R(90,0,0,1),
                        0,{1,0,0}, 0.4f,-0.1f,0.3f,0.2f,0.1f);
    }
}

void drawInnerBody(glm::mat4 m) {
    DrawCtx c;
    renderCylinder(c.prog,c.u, m*T(0,-1,-0.5f)*Sc(1,1,0.4f)*R(-90,1,0,0),
                   c.inner,{1,1,1}, 0.5f,1.8f,4.f);
}

void drawSpine(glm::mat4 m) {
    DrawCtx c;
    renderCube(c.prog,c.u, m*T(0,-0.4f,0),             c.outer,{1,0,1}, 0.1f,0.8f,0.05f);
    renderCube(c.prog,c.u, m*T(0,1,0.2f)*R(20,1,0,0),  c.outer,{1,0,1}, 0.1f,0.6f,0.05f);
    renderCube(c.prog,c.u, m*T(0,2.1f,0.33f)*R(-10,1,0,0), c.outer,{1,0,1}, 0.1f,0.5f,0.05f);
    renderCube(c.prog,c.u, m*T(0,2.9f,0.21f)*R(-5,1,0,0),  c.outer,{1,0,1}, 0.1f,0.3f,0.05f);
    // joints seg 1
    drawSpineJoint(m);
    drawSpineJoint(m*T(0, 0.4f,0));
    drawSpineJoint(m*T(0,-0.4f,0));
    drawSpineJoint(m*T(0,-0.8f,0));
    drawSpineJoint(m*T(0,-1.2f,0));
    // joints seg 2
    drawSpineJoint(m*T(0,1.6f,0.4f));
    drawSpineJoint(m*T(0,1.2f,0.27f));
    drawSpineJoint(m*T(0,0.8f,0.13f));
    // joints seg 3
    drawSpineJoint(m*T(0,2.6f,0.24f));
    drawSpineJoint(m*T(0,2.3f,0.3f));
    drawSpineJoint(m*T(0,1.95f,0.36f));
    // joints seg 4
    drawSpineJoint(m*T(0,3.2f,0.18f));
    drawSpineJoint(m*T(0,2.9f,0.21f));
}

void drawTopBack(glm::mat4 m) {
    DrawCtx c;
    // Original sets blue on cube 1 then leaves glColor3f commented out for the
    // next three, so the persistent GL color stays blue for all four.
    renderCube(c.prog,c.u, m*T( 1,2.3f,-0.2f)*R(-10,1,0,0), c.outer,{0,0,1}, 0.6f,0.7f,0.4f);
    renderCube(c.prog,c.u, m*T(-1,2.3f,-0.2f)*R(-10,1,0,0), c.outer,{0,0,1}, 0.6f,0.7f,0.4f);
    renderCube(c.prog,c.u, m*T( 0.9f,1,-0.2f)*R(20,1,0,0),  c.outer,{0,0,1}, 0.5f,0.6f,0.4f);
    renderCube(c.prog,c.u, m*T(-0.9f,1,-0.2f)*R(20,1,0,0),  c.outer,{0,0,1}, 0.5f,0.6f,0.4f);
    renderTrapezoid(c.prog,c.u, m*T( 0.2f,2.3f,0.05f)*R(-10,1,0,0)*R(-90,0,0,1),
                    c.outer,{0,0,1}, 0.6f,0.1f,0.5f,0.4f,0.35f);
    renderTrapezoid(c.prog,c.u, m*T(-0.2f,2.3f,0.05f)*R(-10,1,0,0)*R(90,0,0,1),
                    c.outer,{0,0,1}, 0.6f,0.1f,0.5f,0.4f,0.35f);
    renderTrapezoid(c.prog,c.u, m*T( 0.9f,2,0.5f)*R(-10,1,0,0), c.outer,{1,0,0}, 1.f,0.f,0.6f,1.5f,0.35f);
    renderTrapezoid(c.prog,c.u, m*T(-0.9f,2,0.5f)*R(-10,1,0,0), c.outer,{1,0,0}, 1.f,0.4f,1.f,1.5f,0.35f);
    drawScale(m*T( 0.7f,2.5f,0.7f)*R(-10,1,0,0)*R(-90,0,1,0));
    drawScale(m*T(-0.7f,2.5f,0.7f)*R(-10,1,0,0)*R(-90,0,1,0));
    renderTrapezoid(c.prog,c.u, m*T( 1.8f,1.5f,-0.2f), c.inner,{1,1,1}, 1.2f,0.f,0.3f,1.f,0.8f);
    renderTrapezoid(c.prog,c.u, m*T(-1.8f,1.5f,-0.2f), c.inner,{1,1,1}, 1.2f,0.9f,1.2f,1.f,0.8f);
}

void drawHeart(glm::mat4 m) {
    DrawCtx c;
    renderPrism(c.prog,c.u, m*T(0,1.9f,-1.5f)*Sc(2.3f,0.7f,1),            texEye,{1,0,0}, 0.25f,0.5f,3);
    renderPrism(c.prog,c.u, m*T(0,1.72f,-1.5f)*Sc(2.3f,0.7f,1)*R(180,0,0,1), texEye,{1,0,0}, 0.25f,0.5f,3);
}

void drawChest(glm::mat4 m) {
    DrawCtx c;
    renderPrism(c.prog,c.u, m*Sc(1.3f,0.7f,1)*T( 0.9f,3.5f,-1.5f)*R(22.5f,0,0,1), c.outer,{1,1,1}, 1.f,1.f,8);
    renderPrism(c.prog,c.u, m*Sc(1.3f,0.7f,1)*T(-0.9f,3.5f,-1.5f)*R(22.5f,0,0,1), c.outer,{1,1,1}, 1.f,1.f,8);
    renderPrism(c.prog,c.u, m*Sc(1,0.4f,1)*T(0,7.5f,-1.5f)*R(180,0,0,1),           c.inner,{1,1,0}, 0.7f,1.f,3);
}

void drawRibs(glm::mat4 m) {
    DrawCtx c;
    renderTrapezoid(c.prog,c.u, m*T( 0.8f,1.7f,-0.8f)*R(-27,0,0,1), c.inner,{1,0,1}, 2.4f,0.5f,2.2f,1.f,0.5f);
    renderTrapezoid(c.prog,c.u, m*T(-0.8f,1.7f,-0.8f)*R( 27,0,0,1), c.inner,{1,0,1}, 2.4f,0.2f,1.9f,1.f,0.5f);
    renderTrapezoid(c.prog,c.u, m*T( 0.3f,1.3f,-1.15f)*R(-24,0,1,0)*R(30,0,0,1), c.outer,{0,1,0}, 0.5f,0.8f,0.8f,1.2f,0.3f);
    renderTrapezoid(c.prog,c.u, m*T(-0.3f,1.3f,-1.15f)*R(-156,0,1,0)*R(30,0,0,1),c.outer,{0,1,0}, 0.5f,0.8f,0.8f,1.2f,0.3f);
}

void drawCore6Packs(glm::mat4 m) {
    DrawCtx c;
    renderCube(c.prog,c.u, m*Sc(3.5f,1.5f,1)*T(0,0.6f,-0.9f)*R(45,0,0,1), c.outer,{0,1,1}, 0.3f,0.3f,0.2f);
    renderTrapezoidGLU(c.prog,c.u,
        m*T(0,0,-1.1f)*R(180,0,1,0)*Sc(3.5f,1.5f,1)*T(0,0.6f,0)*R(45,0,0,1),
        c.outer,{1,1,0}, 0.4f,0.1f,0.3f);
    glm::vec3 ac{0,1,1};
    renderTrapezoid(c.prog,c.u, m*T(-0.7f, 0.28f,-0.5f)*R(-16,0,0,1),              c.outer,ac, 1.3f,0.2f,1.4f,0.3f,1.f);
    renderTrapezoid(c.prog,c.u, m*T( 0.7f, 0.28f,-0.5f)*R(180,0,1,0)*R(-16,0,0,1), c.outer,ac, 1.3f,0.2f,1.4f,0.3f,1.f);
    renderTrapezoid(c.prog,c.u, m*T(-0.6f,-0.15f,-0.5f)*R(-16,0,0,1),              c.outer,ac, 1.2f,0.2f,1.3f,0.3f,1.f);
    renderTrapezoid(c.prog,c.u, m*T( 0.6f,-0.15f,-0.5f)*R(180,0,1,0)*R(-16,0,0,1), c.outer,ac, 1.2f,0.2f,1.3f,0.3f,1.f);
    renderTrapezoid(c.prog,c.u, m*T(-0.5f,-0.6f,-0.5f)*R(-16,0,0,1),               c.outer,ac, 1.1f,0.4f,1.2f,0.3f,1.f);
    renderTrapezoid(c.prog,c.u, m*T( 0.5f,-0.6f,-0.5f)*R(180,0,1,0)*R(-16,0,0,1),  c.outer,ac, 1.1f,0.4f,1.2f,0.3f,1.f);
    drawCoreDetail1(m*T(0,0.15f,-1.1f));
}

void drawPelvis(glm::mat4 m) {
    DrawCtx c;
    renderTrapezoid(c.prog,c.u, m*T(-0.75f,-1,-0.45f)*R(-16,0,0,1),              c.outer,{0,0,1}, 1.5f,0.4f,1.6f,0.3f,1.5f);
    renderTrapezoid(c.prog,c.u, m*T( 0.75f,-1,-0.45f)*R(180,0,1,0)*R(-16,0,0,1), c.outer,{0,0,1}, 1.5f,0.4f,1.6f,0.3f,1.5f);
    renderTrapezoid(c.prog,c.u, m*T(0,-1.6f,-0.45f),                              c.outer,{0,0,1}, 0.5f,0.2f,0.3f,0.8f,1.5f);
    renderCylinder(c.prog,c.u,  m*T(1,-1.5f,-0.5f)*R(-90,0,1,0),                 c.inner,{0.2f,0.2f,0.2f}, 0.1f,0.1f,2.f);
}

void drawShoulder(glm::mat4 m) {
    DrawCtx c;
    renderTrapezoid(c.prog,c.u, m*T(-2,3.2f,-0.5f)*R(180,1,0,0), c.outer,{1,0,0}, 2.5f,2.f,2.f,0.3f,1.5f);
    renderTrapezoid(c.prog,c.u, m*T( 2,3.2f,-0.5f)*R(180,1,0,0), c.outer,{1,0,0}, 2.5f,0.5f,0.5f,0.3f,1.5f);
}

void drawNeck(glm::mat4 m) {
    DrawCtx c;
    ny += 201.f;
    glm::mat4 base = m * T(0,3.2f,-0.5f) * R(ny,0,1,0);
    for (int i = 0; i <= 23; ++i)
        renderTrapezoid(c.prog,c.u,
                        base*R(i*15.f,0,1,0)*T(0.1f,0,0)*R(-90,1,0,0),
                        c.inner,{1,0,1}, 1.2f,0.f,0.f,0.4f,0.4f);
}

// Unused: the original's drawInnerBodyStructure() call is commented out in
// Main.cpp. Ported for parity; kept so the reference stays 1:1.
void drawInnerBodyStructure(glm::mat4 m) {
    DrawCtx c;
    renderTrapezoid(c.prog,c.u, m*T(0, 2.52f,-0.5f), 0,{0,1,0}, 5.f,0.2f,4.8f,1.f,1.5f);
    renderTrapezoid(c.prog,c.u, m*T(0, 1.5f,-0.5f),  0,{0,1,0}, 4.6f,0.8f,3.8f,1.f,1.5f);
    renderTrapezoid(c.prog,c.u, m*T(0, 0,-0.5f),     0,{0,1,0}, 3.f,0.4f,2.6f,2.f,1.5f);
    renderTrapezoid(c.prog,c.u, m*T(0,-1.5f,-0.5f),  0,{0,1,0}, 2.2f,0.9f,1.3f,1.f,1.5f);
}

void drawBody(glm::mat4 m) {
    glm::mat4 base = m * T(0,1,0.5f);
    drawHeart(base);
    drawInnerBody(base);
    drawShoulder(base);
    drawNeck(base);
    drawTopBack(base);
    drawRibs(base);
    drawPelvis(base);
    drawCore6Packs(base);
    drawSpine(base);
    drawChest(base);
}

// ─────────────────────────────────────────────────────────────────────────────
void drawLeftArm(glm::mat4 m) {
    // animation state mutations
    if (leftArmUpBool) {
        if (leftArmRup1 <= 50.f) {
            leftArmRup1 += armRSpeed;
        } else if (leftArmRup <= 15.f || leftArmRup1 <= 110.f) {
            if (!boolWeapon || boolSword) {
                if (leftArmRup <= 15.f) { leftArmRup += armRSpeed/2.f; leftArmRup1 += armRSpeed/2.f; }
                else                    { leftArmRup1 += armRSpeed/2.f; }
            } else {
                if (leftArmRup <= 15.f) {
                    leftArmRup += armRSpeed/2.f;
                    if (leftArmRup1 <= 75.f) leftArmRup1 += armRSpeed/2.f;
                } else {
                    if (leftArmRup1 <= 75.f) leftArmRup1 += armRSpeed/2.f;
                }
            }
        }
    } else if (leftArmDownBool) {
        if (leftArmRup1 >= 50.f) {
            leftArmRup1 -= armRSpeed;
        } else if (leftArmRup1 >= 0.f) {
            if (leftArmRup >= 0.f) { leftArmRup -= armRSpeed/2.f; leftArmRup1 -= armRSpeed/2.f; }
            else                   { leftArmRup1 -= armRSpeed; }
        }
    }
    if (fingerUpBool) {
        if (fingerRup1 <= 90.f && fingerRup <= 90.f) {
            fingerRup += armRSpeed;
            if (!boolHI) fingerRup1 += armRSpeed;
        }
    } else if (fingerDownBool) {
        if (fingerRup1 >= 0.f) fingerRup1 -= armRSpeed;
        if (fingerRup  >= 0.f) fingerRup  -= armRSpeed;
    }
    if (boolSword) {
        if (armLeftBool) {
            if (leftArmRup1 <= 50.f) {
                leftArmRup1 += armRSpeed;
            } else if (leftArmRup <= 15.f || leftArmRup1 <= 110.f) {
                if (leftArmRup <= 15.f) {
                    leftArmRup += armRSpeed/2.f;
                    if (leftArmRup1 <= 110.f) leftArmRup1 += armRSpeed/2.f;
                } else {
                    if (leftArmRup1 <= 110.f) leftArmRup1 += armRSpeed/2.f;
                }
            } else if (armRsword <= 60.f) {
                armRsword += armRSpeed * 2.f;
            } else {
                armLeftBool = false; armRightBool = true;
            }
        } else if (armRightBool) {
            if (armRsword >= 0.f) {
                armRsword -= armRSpeed;
            } else {
                if (leftArmRup1 >= 50.f) { leftArmRup1 -= armRSpeed; }
                else if (leftArmRup1 >= 0.f) {
                    if (leftArmRup >= 0.f) { leftArmRup -= armRSpeed/2.f; leftArmRup1 -= armRSpeed/2.f; }
                    else { leftArmRup1 -= armRSpeed; }
                }
                if (leftArmRup1 <= 0.f) armRightBool = false;
            }
        }
    }
    if (shootBullet) {
        bullet -= armRSpeed/2.f;
        if (bullet >= 10.f) { shootBullet = false; bullet = 0.f; }
    } else {
        bullet = 0.f;
    }

    DrawCtx c;
    glm::mat4 hand = m * T(-0.5f,2.f,0) * R(handLeftAngle,1,0,0);

    // shoulder / upper housing
    renderCube(c.prog,c.u, hand*T(-1.85f,1.8f,0),      c.inner,{1,1,1}, 0.8f,0.4f,0.8f);
    renderCube(c.prog,c.u, hand*T(-1.85f,1.2f,-0.6f),  c.outer,{1,1,1}, 0.8f,0.2f,0.2f);
    renderCube(c.prog,c.u, hand*T(-1.85f,1.2f, 0.6f),  c.outer,{1,1,1}, 0.8f,0.2f,0.2f);
    renderCube(c.prog,c.u, hand*T(-2.55f,1.8f,0),      c.outer,{1,1,1}, 0.3f,0.2f,1.f);
    renderCube(c.prog,c.u, hand*T(-1.65f,1.8f,0),      c.outer,{1,1,1}, 0.6f,0.2f,1.f);
    renderCylinder(c.prog,c.u, hand*T(-2.15f,1.4f,0)*R(90,0,1,0), c.inner,{1,1,1}, 0.2f,0.2f,1.5f);

    // upper arm pivot
    glm::mat4 ua = hand * T(-1.85f,1.4f,0)*R(leftArmRup,1,0,0)*T(1.85f,-1.4f,0);
    renderTrapezoidGLU(c.prog,c.u, ua*T(-1.85f,1.4f,0)*R(90,1,0,0), c.inner,{1,1,1}, 0.4f,0.2f,1.6f);

    // lower arm pivot
    glm::mat4 la = ua * T(-1.85f,0,0)*R(leftArmRup1,1,0,0)*R(armRsword,-1,0,1)*T(1.85f,0,0);
    renderTrapezoidGLU(c.prog,c.u, la*T(-1.85f,0,0)*R(90,1,0,0),    c.outer,{1,1,1}, 0.4f,0.6f,1.6f);
    renderTrapezoidGLU(c.prog,c.u, la*T(-1.85f,-1.2f,0)*R(90,1,0,0),c.outer,{1,1,1}, 0.6f,0.8f,0.2f);
    renderTrapezoidGLU(c.prog,c.u, la*T(-1.85f,-1.4f,0)*R(90,1,0,0),c.outer,{1,1,1}, 0.8f,0.6f,0.6f);
    renderTrapezoidGLU(c.prog,c.u, la*T(-2.f,-0.6f,0)*R(90,-1,-0.5f,0), c.outer,{1,1,0}, 0.3f,0.02f,0.8f);

    // palm pivot (optional HI wrist rotation)
    glm::mat4 palm = la * T(-1.85f,-1.6f,-0.1f);
    if (boolHI) {
        float a = (leftArmRup1 <= 90.f) ? leftArmRup1 : 90.f;
        palm = palm * R(a,0,-1,0);
    }
    palm = palm * T(1.85f,1.6f,0.1f);

    if (!boolWeapon) {
        renderTrapezoidGLU(c.prog,c.u, palm*T(-2.25f,-2.2f,0)*R(90,0,1,0), c.outer,{1,1,1}, 0.2f,0.6f,0.2f);
        renderCube(c.prog,c.u, palm*T(-1.95f,-2.2f,0),                      c.outer,{1,1,1}, 0.1f,0.4f,0.4f);
        renderCube(c.prog,c.u, palm*T(-1.95f,-2.2f,-0.5f)*R(45,1,0,0),      c.outer,{1,1,1}, 0.1f,0.4f,0.1f);

        static const float fz[] = {-0.3f,-0.1f,0.3f,0.1f};
        glm::mat4 f1 = palm*T(-1.95f,-2.6f,-0.3f)*R(fingerRup,0,0,1)*T(1.95f,2.6f,0.3f);
        for (int i = 0; i < 4; ++i) {
            renderCube(c.prog,c.u, f1*T(-1.95f,-2.8f,fz[i]), c.outer,{1,1,1}, 0.1f,0.2f,0.1f);
            renderSphere(c.prog,c.u, f1*T(-1.95f,-2.6f,fz[i]), 0,{0.8f,0.8f,0.8f}, 0.1f);
        }
        glm::mat4 f2 = f1*T(-1.95f,-3.0f,-0.3f)*R(fingerRup,0,0,1)*T(1.95f,3.0f,0.3f);
        for (int i = 0; i < 4; ++i) {
            renderCube(c.prog,c.u, f2*T(-1.95f,-3.2f,fz[i]), c.outer,{1,1,1}, 0.1f,0.2f,0.1f);
            renderSphere(c.prog,c.u, f2*T(-1.95f,-3.0f,fz[i]), 0,{0.8f,0.8f,0.8f}, 0.1f);
        }

        if (boolSword) {
            renderCylinder(c.prog,c.u, palm*T(-1.75f,-2.2f,-0.8f),       c.inner,{1,1,1}, 0.2f,0.2f,1.6f);
            renderTrapezoidGLU(c.prog,c.u, palm*T(-1.75f,-2.2f,0.8f),    c.inner,{1,1,1}, 0.2f,0.4f,0.2f);
            renderTrapezoidGLU(c.prog,c.u, palm*T(-1.75f,-2.2f,1.0f),    c.inner,{1,1,1}, 0.4f,0.2f,0.4f);
            renderTrapezoidGLU(c.prog,c.u, palm*T(-1.75f,-2.2f,-1.4f),   c.inner,{1,1,1}, 0.4f,0.8f,0.4f);
            renderTrapezoidGLU(c.prog,c.u, palm*T(-1.75f,-2.2f,-1.0f),   c.inner,{1,1,1}, 0.8f,0.4f,0.4f);
            renderPrism(c.prog,c.u, palm*T(-1.75f,-2.1f,-5.6f),          c.outer,{1,1,1}, 0.2f,4.2f,3);
            renderPrism(c.prog,c.u, palm*T(-1.75f,-2.3f,-5.6f)*R(60,0,0,1), c.outer,{1,1,1}, 0.2f,4.2f,3);
            renderSwordTip(c.prog,c.u, palm*T(-1.75f,-2.2f,-5.6f), 0,{0.9f,0.9f,1.f});
        }
    } else {
        renderCylinder(c.prog,c.u, palm*T(-1.85f,-1.8f,0)*R(90,1,0,0), c.outer,{1,1,1}, 0.3f,0.3f,0.7f);
        renderSphere(c.prog,c.u,   palm*T(-1.85f,-1.8f+bullet,0),       0,{1,0,0}, 0.28f);
    }
}

void drawRightArm(glm::mat4 m) {
    // animation state mutations
    if (rightArmUpBool) {
        if (rightArmRup1 <= 50.f) {
            rightArmRup1 += armRSpeed;
        } else if (rightArmRup <= 15.f || rightArmRup1 <= 110.f) {
            if (!boolWeapon && !boolSword) {
                if (rightArmRup <= 15.f) { rightArmRup += armRSpeed/2.f; rightArmRup1 += armRSpeed/2.f; }
                else                     { rightArmRup1 += armRSpeed/2.f; }
            } else {
                if (rightArmRup <= 15.f) {
                    rightArmRup += armRSpeed/2.f;
                    if (rightArmRup1 <= 75.f) rightArmRup1 += armRSpeed/2.f;
                } else {
                    if (rightArmRup1 <= 75.f) rightArmRup1 += armRSpeed/2.f;
                }
            }
        }
    } else if (rightArmDownBool) {
        if (rightArmRup1 >= 50.f) {
            rightArmRup1 -= armRSpeed;
        } else if (rightArmRup1 >= 0.f) {
            if (rightArmRup >= 0.f) { rightArmRup -= armRSpeed/2.f; rightArmRup1 -= armRSpeed/2.f; }
            else                    { rightArmRup1 -= armRSpeed; }
        }
    }

    DrawCtx c;
    glm::mat4 hand = m * T(0.5f,2.f,0) * R(handRightAngle,1,0,0);

    renderCube(c.prog,c.u, hand*T(1.85f,1.8f,0),      c.inner,{1,1,1}, 0.8f,0.4f,0.8f);
    renderCube(c.prog,c.u, hand*T(1.85f,1.2f,-0.6f),  c.outer,{1,1,1}, 0.8f,0.2f,0.2f);
    renderCube(c.prog,c.u, hand*T(1.85f,1.2f, 0.6f),  c.outer,{1,1,1}, 0.8f,0.2f,0.2f);
    renderCube(c.prog,c.u, hand*T(2.55f,1.8f,0),      c.outer,{1,1,1}, 0.3f,0.2f,1.f);
    renderCube(c.prog,c.u, hand*T(1.65f,1.8f,0),      c.outer,{1,1,1}, 0.6f,0.2f,1.f);
    renderCylinder(c.prog,c.u, hand*T(0.65f,1.4f,0)*R(90,0,1,0), c.inner,{1,1,1}, 0.2f,0.2f,1.5f);

    // upper arm pivot
    glm::mat4 ua = hand * T(1.85f,1.4f,0)*R(rightArmRup,1,0,0)*T(-1.85f,-1.4f,0);
    renderTrapezoidGLU(c.prog,c.u, ua*T(1.85f,1.4f,0)*R(90,1,0,0), c.inner,{1,1,1}, 0.4f,0.2f,1.6f);

    // lower arm pivot
    glm::mat4 la = ua * T(-1.85f,0,0)*R(rightArmRup1,1,0,0)*T(1.85f,0,0);
    renderTrapezoidGLU(c.prog,c.u, la*T(1.85f,0,0)*R(90,1,0,0),    c.outer,{1,1,1}, 0.4f,0.6f,1.6f);
    renderTrapezoidGLU(c.prog,c.u, la*T(1.85f,-1.2f,0)*R(90,1,0,0),c.outer,{1,1,1}, 0.6f,0.8f,0.2f);
    renderTrapezoidGLU(c.prog,c.u, la*T(1.85f,-1.4f,0)*R(90,1,0,0),c.outer,{1,1,1}, 0.8f,0.6f,0.6f);
    renderTrapezoidGLU(c.prog,c.u, la*T(2.f,-0.6f,0)*R(90,-1,0.5f,0), c.outer,{1,1,0}, 0.3f,0.02f,0.8f);

    if (!boolSword && !boolWeapon) {
        renderTrapezoidGLU(c.prog,c.u, la*T(2.05f,-2.2f,0)*R(90,0,1,0), c.outer,{1,1,1}, 0.6f,0.2f,0.2f);
        renderCube(c.prog,c.u, la*T(1.95f,-2.2f,0),                      c.outer,{1,1,1}, 0.1f,0.4f,0.4f);
        renderCube(c.prog,c.u, la*T(1.95f,-2.2f,-0.5f)*R(45,1,0,0),      c.outer,{1,1,1}, 0.1f,0.4f,0.1f);

        static const float fz[] = {-0.3f,-0.1f,0.3f,0.1f};
        glm::mat4 f1 = la*T(1.95f,-2.6f,-0.3f)*R(fingerRup1,0,0,-1)*T(-1.95f,2.6f,0.3f);
        for (int i = 0; i < 4; ++i) {
            renderCube(c.prog,c.u, f1*T(1.95f,-2.8f,fz[i]), c.outer,{1,1,1}, 0.1f,0.2f,0.1f);
            renderSphere(c.prog,c.u, f1*T(1.95f,-2.6f,fz[i]), 0,{0.8f,0.8f,0.8f}, 0.1f);
        }
        glm::mat4 f2 = f1*T(1.95f,-3.0f,-0.3f)*R(fingerRup1,0,0,-1)*T(-1.95f,3.0f,0.3f);
        for (int i = 0; i < 4; ++i) {
            renderCube(c.prog,c.u, f2*T(1.95f,-3.2f,fz[i]), c.outer,{1,1,1}, 0.1f,0.2f,0.1f);
            renderSphere(c.prog,c.u, f2*T(1.95f,-3.0f,fz[i]), 0,{0.8f,0.8f,0.8f}, 0.1f);
        }
    } else {
        renderCylinder(c.prog,c.u, la*T(1.85f,-1.8f,0)*R(90,1,0,0), c.outer,{1,1,1}, 0.3f,0.3f,0.7f);
        renderSphere(c.prog,c.u,   la*T(1.85f,-1.8f+bullet,0),       0,{1,0,0}, 0.28f);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
void drawLegUpperArmor(glm::mat4 m, float d) {
    DrawCtx c;
    glm::mat4 base = m * Sc(0.3f,0.3f,0.3f);

    // upper armor plates
    glm::mat4 pl = base * T(0,0,-2) * R(-90,1,0,0) * Sc(0.8f,1,0.8f);
    renderTrapezoid(c.prog,c.u, pl,                                       c.outer,{1,1,1}, 1.5f,-0.25f,1.75f,0.5f,9.f);
    renderTrapezoid(c.prog,c.u, pl*T( 2,-1,0)*R(-45,0,0,1)*Sc(0.8f,0.9f,0.8f), c.outer,{1,1,1}, 1.5f,-0.25f,1.75f,0.5f,11.f);
    renderTrapezoid(c.prog,c.u, pl*T(-2,-1,0)*R( 45,0,0,1)*Sc(0.8f,0.9f,0.8f), c.outer,{1,1,1}, 1.5f,-0.25f,1.75f,0.5f,11.f);
    renderTrapezoid(c.prog,c.u, pl*T( 1,-1,-1)*R(-45,0,0,1)*Sc(0.7f,1,0.8f),   c.outer,{1,1,1}, 1.5f,-0.25f,1.75f,0.5f,11.f);
    renderTrapezoid(c.prog,c.u, pl*T(-1,-1,-1)*R( 45,0,0,1)*Sc(0.7f,1,0.8f),   c.outer,{1,1,1}, 1.5f,-0.25f,1.75f,0.5f,11.f);
    renderTrapezoid(c.prog,c.u, pl*T( 1.5f,-2,-1)*R(-45,0,0,1)*Sc(0.7f,1,0.8f),c.outer,{1,1,1}, 1.5f,-0.25f,1.75f,0.5f,11.f);
    renderTrapezoid(c.prog,c.u, pl*T(-1.5f,-2,-1)*R( 45,0,0,1)*Sc(0.7f,1,0.8f),c.outer,{1,1,1}, 1.5f,-0.25f,1.75f,0.5f,11.f);

    // big side plate
    glm::mat4 side = base * T(d*2.5f,0,0)*R(-90,0,1,0)*R(-15,0,0,1)*R(-d*90.f,1,0,0);
    renderTrapezoid(c.prog,c.u, side, c.outer,{1,1,1}, 1.5f,-0.25f,1.75f,0.5f,7.f);

    // cylinders behind thigh (original has outer bound here, like the plates above)
    renderCylinder(c.prog,c.u, base*T(    0,-4,1.3f)*R(-90,1,0,0), c.outer,{1,1,1}, 0.3f,0.3f,7.5f);
    renderCylinder(c.prog,c.u, base*T(-0.5f,-4,1.3f)*R(-90,1,0,0), c.outer,{1,1,1}, 0.3f,0.3f,7.5f);
    renderCylinder(c.prog,c.u, base*T( 0.5f,-4,1.3f)*R(-90,1,0,0), c.outer,{1,1,1}, 0.3f,0.3f,7.5f);
}

void drawLegLowerArmor(glm::mat4 m, float n) {
    DrawCtx c;
    glm::mat4 acc = m * Sc(0.3f,0.3f,0.3f);
    if (n <= 4.f) acc = acc * T(1.5f,0,1.5f);
    if (n >= 8.f) acc = acc * T(0.5f,0,1.2f);
    for (int i = 0; i <= (int)n; ++i) {
        acc = acc * R(360.f/n,0,1,0) * T(1,0,0);
        renderCube(c.prog,c.u, acc, 0,{0.2f,0.3f,0.2f}, 1.f,4.f,0.2f);
    }
}

void drawLegKneeJoint(glm::mat4 m) {
    DrawCtx c;
    float rv = (float)r;
    glm::vec3 col{0.5f,0.3f,0.5f};
    renderPrism(c.prog,c.u, m,              c.inner,col, rv,rv,6);
    renderPrism(c.prog,c.u, m,              c.inner,col, 0.3f*rv,4.f*rv,8);
    renderPrism(c.prog,c.u, m*T(0,0,3),    c.inner,col, rv,rv,6);
    glm::mat4 s1 = m*T(0.875f, 1.5f,0);
    renderPrism(c.prog,c.u, s1,             c.inner,col, rv,rv,6);
    renderPrism(c.prog,c.u, s1,             c.inner,col, 0.3f*rv,4.f*rv,8);
    renderPrism(c.prog,c.u, s1*T(0,0,3),   c.inner,col, rv,rv,6);
    glm::mat4 s2 = m*T(0.875f,-1.5f,0);
    renderPrism(c.prog,c.u, s2,             c.inner,col, rv,rv,6);
    renderPrism(c.prog,c.u, s2,             c.inner,col, 0.3f*rv,4.f*rv,8);
    renderPrism(c.prog,c.u, s2*T(0,0,3),   c.inner,col, rv,rv,6);
}

void drawLegKnee(glm::mat4 m) {
    glm::mat4 base = m * R(-90,0,1,0)*R(10,0,0,1)*Sc(0.13f,0.13f,0.13f);
    drawLegKneeJoint(base);
    drawLegKneeJoint(base*T(3,0,0));
}

void drawLegFoot(glm::mat4 m, float d) {
    DrawCtx c;
    glm::mat4 base = m * R(90,1,0,0)*R(180,0,0,1)*Sc(0.5f,0.5f,0.5f);
    renderCube(c.prog,c.u, base,                                        c.outer,{1,1,1}, 0.75f,0.5f,0.25f);
    renderCylinder(c.prog,c.u, base*T(0,0,-1.75f),                     c.outer,{1,1,1}, 0.2f,0.2f,2.f);
    renderCube(c.prog,c.u, base*T(0,0,-0.5f),                          c.outer,{1,1,1}, 0.75f,0.5f,0.25f);
    renderCube(c.prog,c.u, base*T(0,1,0),                              c.outer,{1,1,1}, 0.75f,0.5f,0.25f);
    renderTrapezoid(c.prog,c.u, base*T(0,2,0),                         c.outer,{1,1,1}, 1.f,-0.25f,1.25f,1.f,0.5f);
    renderTrapezoid(c.prog,c.u, base*T(0,1,-0.5f),                     c.outer,{1,1,1}, 1.f,-0.25f,1.25f,1.f,0.5f);
    renderTrapezoid(c.prog,c.u, base*T(0,0,-1),                        c.outer,{1,1,1}, 1.f,-0.25f,1.25f,1.f,0.5f);
    renderTrapezoid(c.prog,c.u, base*T(0,-1,0)*R(180,0,0,1),           c.outer,{1,1,1}, 1.f,-0.25f,1.25f,1.f,0.5f);
    renderTrapezoid(c.prog,c.u, base*T(-1,0,0)*R( 90,0,0,1),           c.outer,{1,1,1}, 1.f,-0.25f,1.25f,0.5f,0.5f);
    renderTrapezoid(c.prog,c.u, base*T( 1,0,0)*R(270,0,0,1),           c.outer,{1,1,1}, 1.f,-0.25f,1.25f,0.5f,0.5f);
    renderWireSphere(c.prog,c.u, base*T(d*1.25f,0,-0.25f)*R(-d*90.f,0,0,1), 0,{1,1,1}, 0.5f);
}

void drawLeftLeg(glm::mat4 m) {
    glm::mat4 base = m * R(legLeftUpperAngle,1,0,0);

    glm::mat4 thigh = base * T(-0.65f,-2,0);
    drawLegInner(thigh, 10);
    drawLegUpperArmor(thigh, -1);

    glm::mat4 lower = base * T(0,-3.4f,0)*R(legLeftLowerAngle,1,0,0)*T(0,3.4f,0);
    drawLegKnee(lower * T(-0.39f,-3.8f,-0.3f));

    glm::mat4 shin = lower * T(-0.65f,-5,0);
    drawLegInner(shin, 7);
    drawLegLowerArmor(shin, 8);

    drawLegKnee(lower * T(-0.45f,-6.3f,-0.25f) * Sc(0.8f,0.8f,0.8f));
    drawLegFoot(lower * T(-0.65f,-6.8f,0), 1);
}

void drawRightLeg(glm::mat4 m) {
    DrawCtx c;
    glm::mat4 conn = m * T(0,1,0.5f);
    renderTrapezoid(c.prog,c.u, conn*T(0,-1.6f,-0.45f),             c.outer,{0,0,1}, 0.5f,0.2f,0.3f,0.8f,1.5f);
    renderCylinder(c.prog,c.u,  conn*T(1,-1.5f,-0.5f)*R(-90,0,1,0), c.outer,{0.2f,0.2f,0.2f}, 0.1f,0.1f,2.f);

    glm::mat4 base = m * R(leftRightUpperAngle,1,0,0);

    glm::mat4 thigh = base * T(0.65f,-2,0);
    drawLegInner(thigh, 10);
    drawLegUpperArmor(thigh, 1);

    glm::mat4 lower = base * T(0,-3.4f,0)*R(legRightLowerAngle,1,0,0)*T(0,3.4f,0);
    drawLegKnee(lower * T(0.91f,-3.8f,-0.3f));

    glm::mat4 shin = lower * T(0.65f,-5,0);
    drawLegInner(shin, 7);
    drawLegLowerArmor(shin, 8);

    drawLegKnee(lower * T(0.85f,-6.3f,-0.25f) * Sc(0.8f,0.8f,0.8f));
    drawLegFoot(lower * T(0.65f,-6.8f,0), -1);
}

void drawOcean(glm::mat4 m) {
    DrawCtx c;
    renderDisk(c.prog,c.u, m*R(90,1,0,0)*T(0,0,6), texOcean,{1,1,1}, 0.f,20.f,50,50);
}

void drawSkyBox(glm::mat4 m) {
    DrawCtx c;
    renderSphere(c.prog,c.u, m*R(90,1,0,0), texSky,{1,1,1}, 100.f);
}
