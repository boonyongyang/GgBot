#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "shader.h"
#include "texture.h"
#include "input.h"
#include "scene.h"
#include "state.h"

// Make the process CWD the directory containing the executable, so the relative
// "shaders/" and "textures/" paths resolve no matter where GgBot is launched from
// (the CMake POST_BUILD step copies both next to the binary).
static void chdirToExecutable() {
    char buf[4096];
#if defined(__APPLE__)
    uint32_t size = sizeof(buf);
    if (_NSGetExecutablePath(buf, &size) != 0) return;
#elif defined(_WIN32)
    DWORD n = GetModuleFileNameA(nullptr, buf, sizeof(buf));
    if (n == 0 || n >= sizeof(buf)) return;
#else
    ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (n <= 0) return;
    buf[n] = '\0';
#endif
    std::string path(buf);
    size_t slash = path.find_last_of("/\\");
    if (slash == std::string::npos) return;
    std::string dir = path.substr(0, slash);
#if defined(_WIN32)
    SetCurrentDirectoryA(dir.c_str());
#else
    if (chdir(dir.c_str()) != 0) { /* fall back to the current directory */ }
#endif
}

// Fail early with a clear message if the assets aren't found, instead of rendering
// a black window (missing shaders throw; missing textures would silently be 0).
static bool assetsPresent() {
    const char* required[] = {
        "shaders/ggbot.vert", "shaders/ggbot.frag", "textures/metal2.bmp",
    };
    for (const char* p : required) {
        std::ifstream f(p);
        if (!f.good()) {
            fprintf(stderr,
                    "GgBot: required asset '%s' not found.\n"
                    "Run the built binary from its own directory (shaders/ and\n"
                    "textures/ are copied next to it), e.g. ./build/GgBot.\n", p);
            return false;
        }
    }
    return true;
}

static void framebufferSizeCallback(GLFWwindow* /*w*/, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    chdirToExecutable();
    if (!assetsPresent()) return 1;

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

    // GGBOT_MAX_FRAMES>0 renders that many frames then exits — used for the CI
    // headless smoke test; unset/0 runs normally until the window is closed.
    const char* mf = getenv("GGBOT_MAX_FRAMES");
    long maxFrames = mf ? atol(mf) : 0;
    long frame = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &w, &h);
        display(prog, u, w, h);
        glfwSwapBuffers(window);
        glfwPollEvents();
        if (maxFrames > 0 && ++frame >= maxFrames) break;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
