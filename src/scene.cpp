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
    float aspect = (height > 0) ? (float)width / height : 1.f;
    if (isOrtho) {
        proj = glm::ortho(-10.f * aspect, 10.f * aspect,
                          -10.f, 10.f, orthoNear, orthoFar);
    } else {
        proj = glm::perspective(glm::radians(45.f), aspect, 0.1f, 500.f);
    }

    // camera: zoom + mouse orbit
    view = glm::mat4(1.f);
    view = glm::translate(view, {0.f, 0.f, perspecZoomLevel});
    view = glm::rotate(view, glm::radians(mouseXRotate), {1,0,0});
    view = glm::rotate(view, glm::radians(mouseYRotate), {0,1,0});
    view = glm::rotate(view, glm::radians(mouseZRotate), {0,0,1});

    // panning
    view = glm::translate(view, {ptX, ptY, 0.f});
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

    glm::mat4 world = glm::mat4(1.f);
    world = glm::rotate(world, glm::radians(faceAngle), {0,1,0});

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
