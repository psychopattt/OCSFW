#pragma once

class Simulation;
class ImGuiWindow;
class MouseHandler;
class KeyboardHandler;

class OCSFW
{
	public:
		OCSFW(Simulation* simulation);
		OCSFW& WithTitle(const char* title);
		OCSFW& WithSize(int width, int height);
		OCSFW& WithMouseHandler(MouseHandler* mouseHandler);
		OCSFW& WithKeyboardHandler(KeyboardHandler* keyboardHandler);
		OCSFW& WithMenus(ImGuiWindow* menus[], size_t menuCount);
		void Run();

	private:
		int width = 1280;
		int height = 720;
		const char* title = "OCSFW";
};
