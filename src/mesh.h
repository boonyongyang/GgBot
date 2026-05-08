#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Interleaved vertex: position(3) + normal(3) + texcoord(2) = 8 floats
struct MeshData {
    std::vector<float>        vertices;
    std::vector<unsigned int> indices;
    GLuint vao = 0, vbo = 0, ebo = 0;
};

void uploadMesh(MeshData& m);
void drawMesh(const MeshData& m);

// ── Cached primitive accessors ───────────────────────────────────────────────
// Each returns a reference to a statically cached, GPU-uploaded mesh.

MeshData& getCubeMesh(float x, float y, float z);
MeshData& getTrapezoidMesh(float top, float bot1, float bot2, float y, float z);
MeshData& getSphereMesh(float r, int slices = 50, int stacks = 50);
// cylinder with caps; topR==0 gives cone
MeshData& getCylinderMesh(float baseR, float topR, float h, int slices = 50);
MeshData& getDiskMesh(float inR, float outR, int slices, int loops);
// prism/polygon: uniform-radius cylinder with `sides` faces
MeshData& getPrismMesh(float l, float h, int sides);
// wireframe sphere approximation (foot decoration)
MeshData& getWireSphereMesh(float r, int slices = 30, int stacks = 30);
// 4 hardcoded triangles for the sword tip
MeshData& getSwordTipMesh();

// ── High-level draw helpers (set uniforms + draw) ───────────────────────────
// These correspond 1-to-1 with the original render* functions.
// `prog`  — active shader program (for setDrawUniforms)
// `u`     — cached uniform locations
// `model` — current model matrix
// `texID` — 0 = no texture (use color only)
// `color` — glm::vec3 RGB

#include "shader.h"

void renderCube      (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color, float x, float y, float z);
void renderTrapezoid (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color,
                      float top, float bot1, float bot2, float hy, float hz);
void renderSphere    (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color, float r);
void renderCylinder  (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color,
                      float baseR, float topR, float h);
void renderDisk      (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color,
                      float inR, float outR, int slices, int loops);
void renderPrism     (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color,
                      float l, float h, int sides);
void renderCone      (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color, float topR, float h);
// renderTrapezoidGLU — GLU-based 4-sided frustum (was renderTrapezoid in original)
void renderTrapezoidGLU(GLuint prog, const Uniforms& u, glm::mat4 model,
                        GLuint texID, glm::vec3 color,
                        float baseL, float topL, float h);
void renderCuboidGLU (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color, float l, float h);
void renderWireSphere(GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color, float r);
void renderSwordTip  (GLuint prog, const Uniforms& u, glm::mat4 model,
                      GLuint texID, glm::vec3 color);

// Called once per frame by scene.cpp before drawing
void setFrameContext(GLuint prog, const Uniforms& u,
                     const glm::mat4& view, const glm::mat4& proj);
void getFrameContext(GLuint& prog, Uniforms& u,
                     glm::mat4& view, glm::mat4& proj);
