#pragma once
#include <glm/glm.hpp>
#include "shader.h"

// Every draw function receives the accumulated model matrix from its parent.
// Textures and colors are resolved internally from global state (state.h).

void drawSpineJoint      (glm::mat4 model);
void drawScale           (glm::mat4 model);
void drawEye             (glm::mat4 model);
void drawNoseAndMouth    (glm::mat4 model);
void drawEar             (glm::mat4 model);
void drawHat             (glm::mat4 model);
void drawStructureHead   (glm::mat4 model);
void drawLegInnerNerve   (glm::mat4 model, float r, float h);
void drawCoreDetail1     (glm::mat4 model);
void drawInnerBody       (glm::mat4 model);
void drawSpine           (glm::mat4 model);
void drawTopBack         (glm::mat4 model);
void drawHeart           (glm::mat4 model);
void drawChest           (glm::mat4 model);
void drawRibs            (glm::mat4 model);
void drawCore6Packs      (glm::mat4 model);
void drawPelvis          (glm::mat4 model);
void drawShoulder        (glm::mat4 model);
void drawNeck            (glm::mat4 model);
void drawInnerBodyStructure(glm::mat4 model);
void drawHead            (glm::mat4 model);
void drawBody            (glm::mat4 model);
void drawLeftArm         (glm::mat4 model);
void drawRightArm        (glm::mat4 model);
void drawLegInner        (glm::mat4 model, float h);
void drawLegUpperArmor   (glm::mat4 model, float d);
void drawLegLowerArmor   (glm::mat4 model, float n);
void drawLegKneeJoint    (glm::mat4 model);
void drawLegKnee         (glm::mat4 model);
void drawLegFoot         (glm::mat4 model, float d);
void drawLeftLeg         (glm::mat4 model);
void drawRightLeg        (glm::mat4 model);
void drawOcean           (glm::mat4 model);
void drawSkyBox          (glm::mat4 model);
