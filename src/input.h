#pragma once
#include <glad/glad.h>   // must precede GLFW so it suppresses GLFW's own GL include
#include <GLFW/glfw3.h>

void registerCallbacks(GLFWwindow* window);
