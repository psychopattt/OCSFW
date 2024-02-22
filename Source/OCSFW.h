#pragma once

class Simulation;
class ImGuiWindow;
class MouseHandler;
class KeyboardHandler;

class OCSFW
{
	public:
		OCSFW(Simulation* simulation, const char* title);
		OCSFW(Simulation* simulation, const char* title, MouseHandler* mouseHandler,
			KeyboardHandler* keyboardHandler);
		OCSFW(Simulation* simulation, const char* title, ImGuiWindow* menus[],
			size_t menuCount);
		OCSFW(Simulation* simulation, const char* title, MouseHandler* mouseHandler,
			KeyboardHandler* keyboardHandler, ImGuiWindow* menus[], size_t menuCount);
};
