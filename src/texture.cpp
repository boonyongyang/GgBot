#include "texture.h"
#include "state.h"
#include <cstdio>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texEye   = 0;
GLuint texMetal = 0;
GLuint texOcean = 0;
GLuint texSky   = 0;

GLuint loadTexture(const char* path) {
    int w, h, channels;
    // BMP origin is bottom-left, same as OpenGL — flip so texture is right-side up
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &channels, 3);
    if (!data) {
        fprintf(stderr, "Failed to load texture: %s\n", path);
        return 0;
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return tex;
}

void loadAllTextures() {
    // Outer armor textures (cycled with K key)
    textureArrOuter[0] = loadTexture("textures/metal2.bmp");
    textureArrOuter[1] = loadTexture("textures/camoTexture.bmp");
    textureArrOuter[2] = loadTexture("textures/armorMetal.bmp");
    textureArrOuter[3] = loadTexture("textures/armorPattern3.bmp");
    textureArrOuter[4] = loadTexture("textures/complexTexture.bmp");

    // Inner textures (cycled with M key)
    textureArrInner[0] = loadTexture("textures/darksteel32.bmp");
    textureArrInner[1] = loadTexture("textures/armorPattern2.bmp");
    textureArrInner[2] = loadTexture("textures/armorPattern.bmp");

    // Specific-use textures
    texEye   = loadTexture("textures/eyetest.bmp");
    texMetal = textureArrOuter[0];   // same image as metal2.bmp — reuse, don't reload
    texOcean = loadTexture("textures/ocean.bmp");
    texSky   = loadTexture("textures/sky.bmp");
}
