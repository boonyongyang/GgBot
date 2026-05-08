#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

#include "shader.h"
#include "texture.h"
#include "input.h"
#include "scene.h"
#include "state.h"

static void framebufferSizeCallback(GLFWwindow* /*w*/, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "glfwInit failed\n");
        return 1;
    }

    // OpenGL 3.3 Core Profile (required on macOS Apple Silicon)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "GgBot", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "glfwCreateWindow failed\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "gladLoadGLLoader failed\n");
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    registerCallbacks(window);

    GLuint prog = loadShaderProgram("shaders/ggbot.vert", "shaders/ggbot.frag");
    if (!prog) {
        glfwTerminate();
        return 1;
    }
    Uniforms u = cacheUniforms(prog);

    loadAllTextures();

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &w, &h);
        display(prog, u, w, h);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
