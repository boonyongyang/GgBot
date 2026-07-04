#include "scene.h"
#include "state.h"
#include "mesh.h"
#include "robot.h"
#include "texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void computeProjection(int width, int height, glm::mat4& view, glm::mat4& proj) {
    (void)width; (void)height;   // original ignores window aspect

    // PROJECTION — replicates the original projection() exactly: panning, a 180°
    // flip ("reduces distortion"), and prY are baked into the projection matrix,
    // and the perspective path stacks gluPerspective(35,1,-1,1) onto glFrustum.
    proj = glm::mat4(1.f);
    proj = glm::translate(proj, {ptX, ptY, 0.f});
    proj = glm::rotate(proj, glm::radians(180.f), {0,1,0});
    proj = glm::rotate(proj, glm::radians(prY),   {0,1,0});
    if (isOrtho) {
        proj = proj * glm::ortho(-10.f, 10.f, -10.f, 10.f, orthoNear, orthoFar);
    } else {
        proj = proj * glm::perspective(glm::radians(35.f), 1.f, -1.f, 1.f)
                    * glm::frustum(-10.f, 10.f, -10.f, 10.f, perspecNear, perspecFar);
    }

    // MODELVIEW — replicates display(): zoom, mouse orbit, then faceAngle.
    // In ortho the original forces perspecZoomLevel = 8 each frame (scene1).
    float zoom = isOrtho ? 8.f : perspecZoomLevel;
    view = glm::mat4(1.f);
    view = glm::translate(view, {0.f, 0.f, zoom});
    view = glm::rotate(view, glm::radians(mouseXRotate), {1,0,0});
    view = glm::rotate(view, glm::radians(mouseYRotate), {0,1,0});
    view = glm::rotate(view, glm::radians(mouseZRotate), {0,0,1});
    view = glm::rotate(view, glm::radians(faceAngle),    {0,1,0});
}

void setLighting(GLuint prog, const Uniforms& u) {
    glUniform1i(u.lightOn,    isLightOn  ? 1 : 0);
    glUniform1i(u.ambientOn,  isAmbientOn  ? 1 : 0);
    glUniform1i(u.diffuseOn,  isDiffuseOn  ? 1 : 0);
    glUniform1i(u.specularOn, isSpecularOn ? 1 : 0);

    glUniform3fv(u.ambientColor,  1, amb);
    glUniform3f (u.ambientPos,    posA[0], posA[1], posA[2]);
    glUniform3fv(u.diffuseColor,  1, dif);
    glUniform3f (u.diffusePos,    posD[0], posD[1], posD[2]);
    glUniform3fv(u.materialAmbient, 1, ambM);
    glUniform3fv(u.materialDiffuse, 1, difM);
    glUniform1f (u.materialFv,    materialFv);
}

void summonGgBot(glm::mat4 model) {
    // global robot position / yaw
    model = glm::translate(model, {tX, tY, tZ});
    model = glm::rotate(model, glm::radians(rY), {0,1,0});
    rY += rSpeed;

    // upper body (rotates independently for attack360)
    glm::mat4 upper = glm::rotate(model, glm::radians(rBody), {0,1,0});
    rBody += rBodySpeed;

    drawHead(upper);
    drawBody(upper);
    drawRightArm(upper);
    drawLeftArm(upper);

    drawLeftLeg(model);
    drawRightLeg(model);
}

void scene1(GLuint prog, const Uniforms& u, int width, int height) {
    glm::mat4 view, proj;
    computeProjection(width, height, view, proj);
    setFrameContext(prog, u, view, proj);
    setLighting(prog, u);

    // faceAngle is applied in the modelview (see computeProjection), matching the
    // original display(); scene1's world matrix therefore starts at identity.
    glm::mat4 world = glm::mat4(1.f);

    drawSkyBox(world);
    if (!isOrtho) drawOcean(world);

    glm::mat4 robotBase = glm::translate(world, {0.f, 0.8f, 0.f});
    summonGgBot(robotBase);
}

void display(GLuint prog, const Uniforms& u, int width, int height) {
    glClearColor(0.5f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(prog);
    scene1(prog, u, width, height);
}
