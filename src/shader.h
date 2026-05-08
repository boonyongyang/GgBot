#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Uniforms {
    // matrices
    GLint model, view, projection, normalMatrix;
    // draw-call
    GLint color, useTexture, texture;
    // lighting toggles
    GLint lightOn, ambientOn, diffuseOn, specularOn;
    // light params
    GLint ambientColor, ambientPos;
    GLint diffuseColor, diffusePos;
    // material
    GLint materialAmbient, materialDiffuse, materialFv;
};

GLuint   loadShaderProgram(const char* vertPath, const char* fragPath);
Uniforms cacheUniforms(GLuint prog);

// Call once per frame to push lighting state
void setLightingUniforms(GLuint prog, const Uniforms& u);

// Call before every draw call
void setModelUniforms(GLuint prog, const Uniforms& u,
                      const glm::mat4& model,
                      const glm::mat4& view,
                      const glm::mat4& proj);

void setDrawUniforms(GLuint prog, const Uniforms& u,
                     GLuint texID, const glm::vec3& color);
