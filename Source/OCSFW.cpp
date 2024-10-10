#include "OCSFW.h"

#include "Interface/Interface.h"
#include "Settings/UpdateType.h"
#include "Simulation/Simulation.h"
#include "Interface/ImGui/ImGuiWindow/ImGuiWindow.h"
#include "Inputs/KeyboardHandler/KeyboardHandler.h"
#include "Inputs/MouseHandler/MouseHandler.h"
#include "Settings/InputHandlers.h"
#include "Settings/MainSettings.h"
#include "Settings/ImGuiMenus.h"

OCSFW::OCSFW(Simulation* simulation)
{
	MainSettings::Sim = simulation;
}

OCSFW& OCSFW::WithTitle(const char* title)
{
	this->title = title;
	return *this;
}

OCSFW& OCSFW::WithSize(int width, int height)
{
	this->height = height;
	this->width = width;
	return *this;
}

OCSFW& OCSFW::WithMouseHandler(MouseHandler* mouseHandler)
{
	InputHandlers::MouseHandler = mouseHandler;
	return *this;
}

OCSFW& OCSFW::WithKeyboardHandler(KeyboardHandler* keyboardHandler)
{
	InputHandlers::KeyboardHandler = keyboardHandler;
	return *this;
}

OCSFW& OCSFW::WithMenus(ImGuiWindow* menus[], size_t menuCount)
{
	ImGuiMenus::MenuCount = menuCount;
	ImGuiMenus::Menus = menus;
	return *this;
}

void OCSFW::Run()
{
	Interface gui(width, height, title);
	MainSettings::Sim->Initialize();

	while (!gui.ShouldExit())
	{
		UpdateType updateType = gui.Update();

		if (updateType & SimulationUpdate)
			MainSettings::Sim->Execute();

		if (updateType & DisplayUpdate)
			MainSettings::Sim->Draw();
	}
}
