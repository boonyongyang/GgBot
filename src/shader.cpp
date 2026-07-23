#include "shader.h"
#include "state.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstdio>

static std::string readFile(const char* path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error(std::string("Cannot open shader: ") + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static GLuint compileShader(GLenum type, const char* src) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    GLint ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        fprintf(stderr, "Shader compile error:\n%s\n", log);
        throw std::runtime_error("Shader compile failed");
    }
    return id;
}

GLuint loadShaderProgram(const char* vertPath, const char* fragPath) {
    std::string vSrc = readFile(vertPath);
    std::string fSrc = readFile(fragPath);
    GLuint vert = compileShader(GL_VERTEX_SHADER,   vSrc.c_str());
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fSrc.c_str());
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);
    GLint ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(prog, 512, nullptr, log);
        fprintf(stderr, "Shader link error:\n%s\n", log);
        throw std::runtime_error("Shader link failed");
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
    return prog;
}

Uniforms cacheUniforms(GLuint prog) {
    Uniforms u{};
    auto loc = [&](const char* name) { return glGetUniformLocation(prog, name); };
    u.model          = loc("uModel");
    u.view           = loc("uView");
    u.projection     = loc("uProjection");
    u.normalMatrix   = loc("uNormalMatrix");
    u.color          = loc("uColor");
    u.useTexture     = loc("uUseTexture");
    u.texture        = loc("uTexture");
    u.lightOn        = loc("uLightOn");
    u.ambientOn      = loc("uAmbientOn");
    u.diffuseOn      = loc("uDiffuseOn");
    u.specularOn     = loc("uSpecularOn");
    u.ambientColor   = loc("uAmbientColor");
    u.ambientPos     = loc("uAmbientPos");
    u.diffuseColor   = loc("uDiffuseColor");
    u.diffusePos     = loc("uDiffusePos");
    u.materialAmbient = loc("uMaterialAmbient");
    u.materialDiffuse = loc("uMaterialDiffuse");
    u.materialFv     = loc("uMaterialFv");
    u.legacyLook     = loc("uLegacyLook");
    return u;
}

void setLightingUniforms(GLuint /*prog*/, const Uniforms& u) {
    glUniform1i(u.lightOn,    isLightOn);
    glUniform1i(u.ambientOn,  isAmbientOn);
    glUniform1i(u.diffuseOn,  isDiffuseOn);
    glUniform1i(u.specularOn, isSpecularOn);
    glUniform3fv(u.ambientColor,    1, amb);
    glUniform3fv(u.ambientPos,      1, posA);
    glUniform3fv(u.diffuseColor,    1, dif);
    glUniform3fv(u.diffusePos,      1, posD);
    glUniform3fv(u.materialAmbient, 1, ambM);
    glUniform3fv(u.materialDiffuse, 1, difM);
    glUniform1f(u.materialFv, materialFv);
    // bind texture unit 0
    glUniform1i(u.texture, 0);
}

void setModelUniforms(GLuint /*prog*/, const Uniforms& u,
                      const glm::mat4& model,
                      const glm::mat4& view,
                      const glm::mat4& proj) {
    glUniformMatrix4fv(u.model,      1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(u.view,       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(u.projection, 1, GL_FALSE, glm::value_ptr(proj));
    glm::mat3 nm = glm::mat3(glm::transpose(glm::inverse(model)));
    glUniformMatrix3fv(u.normalMatrix, 1, GL_FALSE, glm::value_ptr(nm));
}

void setDrawUniforms(GLuint /*prog*/, const Uniforms& u,
                     GLuint texID, const glm::vec3& color) {
    glUniform3fv(u.color, 1, glm::value_ptr(color));
    if (texID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        glUniform1f(u.useTexture, 1.f);
    } else {
        glUniform1f(u.useTexture, 0.f);
    }
}
