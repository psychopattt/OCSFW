#include "InputHandler.h"

#include "GLFW/glfw3.h"

#include "MouseHandler/MouseHandler.h"
#include "KeyboardHandler/KeyboardHandler.h"
#include "Settings/InputHandlers.h"

using std::make_unique;

InputHandler::InputHandler()
{
	if (InputHandlers::MouseHandler == nullptr)
	{
		mouseHandler = make_unique<MouseHandler>();
		InputHandlers::MouseHandler = mouseHandler.get();
	}
	
	if (InputHandlers::KeyboardHandler == nullptr)
	{
		keyboardHandler = make_unique<KeyboardHandler>();
		InputHandlers::KeyboardHandler = keyboardHandler.get();
	}
}

void InputHandler::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	InputHandlers::MouseHandler->HandleMouseScroll(window, offsetX, offsetY);
}

void InputHandler::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	InputHandlers::MouseHandler->HandleMouseButton(window, button, action, mods);
}

void InputHandler::HandleKeyboard(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	InputHandlers::KeyboardHandler->HandleKeyboard(window, key, scanCode, action, mods);
}

void InputHandler::Update()
{
	double deltaTime = ComputeDeltaTime();
	InputHandlers::KeyboardHandler->Update(deltaTime);
	InputHandlers::MouseHandler->Update(deltaTime);
}

double InputHandler::ComputeDeltaTime()
{
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	return deltaTime;
}

InputHandler::~InputHandler() { }
