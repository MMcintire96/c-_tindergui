#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imstb_rectpack.h"
#include "imstb_truetype.h"

//GLFWwindow *window = Null;
GLFWwindow *initglfwWindow();
GLFWwindow *pollWindow(GLFWwindow  *window);
GLFWwindow *renderWindow(GLFWwindow *window);
GLFWwindow *cleanWindow(GLFWwindow *window);
ImVec4 rgba(unsigned char r, unsigned char g, unsigned char b, float a);
