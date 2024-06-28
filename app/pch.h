#pragma once


// Graphics
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#undef APIENTRY

// ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Standard Libraries
#include <string>
#include <exception>
#include <unordered_map>
#include <filesystem>

// StegPass Configuration
#include "Config.hpp"