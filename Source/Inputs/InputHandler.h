#pragma once

#include <memory>

using std::unique_ptr;

struct GLFWwindow;

class InputHandler
{
	public:
		InputHandler();
		void HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY);
		void HandleMouseButton(GLFWwindow* window, int button, int action, int mods);
		void HandleKeyboard(GLFWwindow* window, int key, int scanCode, int action, int mods);
		void Update();
		~InputHandler();

	private:
		double lastTime = 0;
		unique_ptr<class MouseHandler> mouseHandler = nullptr;
		unique_ptr<class KeyboardHandler> keyboardHandler = nullptr;

		double ComputeDeltaTime();
};
