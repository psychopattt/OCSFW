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

OCSFW::OCSFW(Simulation* simulation, const char* title) :
	OCSFW(simulation, title, nullptr, nullptr, nullptr, 0) { }

OCSFW::OCSFW(Simulation* simulation, const char* title, MouseHandler* mouseHandler,
	KeyboardHandler* keyboardHandler) : OCSFW(simulation, title, mouseHandler,
	keyboardHandler, nullptr, 0) { }

OCSFW::OCSFW(Simulation* simulation, const char* title, ImGuiWindow* menus[],
	size_t menuCount) : OCSFW(simulation, title, nullptr, nullptr, menus,
	menuCount) { }

OCSFW::OCSFW(Simulation* simulation, const char* title, MouseHandler* mouseHandler,
	KeyboardHandler* keyboardHandler, ImGuiWindow* menus[], size_t menuCount)
{
	MainSettings::Sim = simulation;
	InputHandlers::MouseHandler = mouseHandler;
	InputHandlers::KeyboardHandler = keyboardHandler;
	ImGuiMenus::MenuCount = menuCount;
	ImGuiMenus::Menus = menus;

	Interface gui(1280, 720, title);
	simulation->Initialize();

	while (!gui.ShouldExit())
	{
		UpdateType updateType = gui.Update();

		if (updateType & SimulationUpdate)
			simulation->Execute();

		if (updateType & DisplayUpdate)
			simulation->Draw();
	}
}
