#include "Interface.h"

#include "GLFW/glfw3.h"

#include "Settings/LogString/LogString.h"
#include "Inputs/InputHandler.h"
#include "Settings/MainSettings.h"
#include "Viewport/Viewport.h"

void Interface::ActivateCallbacks()
{
	glfwSetErrorCallback(ErrorCallback);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetFramebufferSizeCallback(window, ResizeCallback);
}

void Interface::ErrorCallback(int error, const char* message)
{
	MainSettings::Log << "GLFW Error - Code " << error << "\n" <<
		message << "\n";
}

void Interface::ResizeCallback(GLFWwindow* window, int width, int height)
{
	Interface* gui = reinterpret_cast<Interface*>(glfwGetWindowUserPointer(window));
	gui->height = height;
	gui->width = width;
	gui->viewport->ComputeSettings();
}

void Interface::MouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	Interface* gui = reinterpret_cast<Interface*>(glfwGetWindowUserPointer(window));
	gui->inputHandler->HandleMouseScroll(window, offsetX, offsetY);
}

void Interface::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Interface* gui = reinterpret_cast<Interface*>(glfwGetWindowUserPointer(window));
	gui->inputHandler->HandleMouseButton(window, button, action, mods);
}

void Interface::KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	Interface* gui = reinterpret_cast<Interface*>(glfwGetWindowUserPointer(window));
	gui->inputHandler->HandleKeyboard(window, key, scanCode, action, mods);
}
