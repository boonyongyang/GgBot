#pragma once
#include <glad/glad.h>

// Extra textures used by specific draw functions
extern GLuint texEye;
extern GLuint texMetal;
extern GLuint texOcean;
extern GLuint texSky;

GLuint loadTexture(const char* path);
void   loadAllTextures();
