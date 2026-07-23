#pragma once
#include <glm/glm.hpp>
#include "shader.h"

void computeProjection(int width, int height, glm::mat4& view, glm::mat4& proj);
void setLighting(GLuint prog, const Uniforms& u);
void summonGgBot(glm::mat4 model);
void scene1(GLuint prog, const Uniforms& u, int width, int height);
void display(GLuint prog, const Uniforms& u, int width, int height);
