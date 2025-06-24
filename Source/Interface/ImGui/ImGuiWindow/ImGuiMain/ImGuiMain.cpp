#include "ImGuiMain.h"

#include <string>

#include "imgui/imgui.h"

#include "Settings/TransformSettings.h"
#include "Simulation/Simulation.h"
#include "Interface/Interface.h"
#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"

using namespace ImGui;

static const char* fontSizes[] = {
	"Small", "Medium", "Large", "Very Large"
};

void ImGuiMain::Initialize()
{
	MainSettings::PendingSimSeed = MainSettings::Sim->GetSeed();
	MainSettings::PendingSimSize[0] = MainSettings::Sim->GetWidth();
	MainSettings::PendingSimSize[1] = MainSettings::Sim->GetHeight();
}

void ImGuiMain::Render()
{
	SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(250, -1), ImGuiCond_FirstUseEver);

	if (Begin("Settings"))
	{
		PushItemWidth(-1);
		RenderSimulationSection();
		RenderPerformanceSection();
		RenderInterfaceSection();
		PopItemWidth();
	}

	End();
}

void ImGuiMain::RenderSimulationSection()
{
	if (CollapsingHeader("Simulation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		RenderSimulationCopyMenu();
		RenderSimulationSettingsSection();
		RenderSimulationPositionSection();
		RenderSimulationZoomSection();
	}
}

void ImGuiMain::RenderSimulationCopyMenu()
{
	if (BeginPopupContextItem())
	{
		RenderSimulationCopyButton();
		RenderSimulationPasteButton();
		EndPopup();
	}
}

void ImGuiMain::RenderSimulationCopyButton()
{
	if (MenuItem("Copy"))
	{
		SetClipboardText((
			std::to_string(MainSettings::PendingSimSize[0]) + ',' +
			std::to_string(MainSettings::PendingSimSize[1]) + ',' +
			std::to_string(MainSettings::PendingSimSeed) + ',' +
			std::to_string(TransformSettings::PanX) + ',' +
			std::to_string(TransformSettings::PanY) + ',' +
			std::to_string(TransformSettings::Zoom)
		).c_str());
	}
}

void ImGuiMain::RenderSimulationPasteButton()
{
	if (MenuItem("Paste"))
	{
		const char* clipboardChars = GetClipboardText();
		std::string clipboard = clipboardChars != NULL ? clipboardChars : "";

		try
		{
			std::string settings[6];
			size_t settingPosition = 0;

			for (int i = 0; i < std::size(settings); i++)
			{
				size_t delimiterPosition = clipboard.find(',', settingPosition);
				size_t settingLength = delimiterPosition - settingPosition;
				settings[i] = clipboard.substr(settingPosition, settingLength);
				settingPosition = delimiterPosition + 1;
			}

			TransformSettings::Zoom = static_cast<unsigned short>(std::stoul(settings[5]));
			TransformSettings::ZoomOnMouse = false;
			TransformSettings::PanY = std::stoll(settings[4]);
			TransformSettings::PanX = std::stoll(settings[3]);
			MainSettings::PendingSimSeed = std::stoul(settings[2]);
			MainSettings::PendingSimSize[1] = std::stoi(settings[1]);
			MainSettings::PendingSimSize[0] = std::stoi(settings[0]);
		}
		catch (...)
		{
			MainSettings::Log << "Simulation Error - Failed to parse settings\n" <<
				"Clipboard content: " << clipboard << "\n\n";
		}
	}
}

void ImGuiMain::RenderSimulationSettingsSection()
{
	using MainSettings::PendingSimSize, MainSettings::PendingSimSeed;

	if (Button("Restart", ImVec2(-1, 0)))
	{
		MainSettings::Sim->Restart(PendingSimSize[0], PendingSimSize[1], PendingSimSeed);
		MainSettings::Gui->NotifyRestart();
	}

	SeparatorText("Size");
	DragInt2("##dragSize", PendingSimSize, 1.0f, 0, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

	SeparatorText("Seed");
	InputScalar("##textSeed", ImGuiDataType_U32, &PendingSimSeed, &seedStepSpeed);
}

void ImGuiMain::RenderSimulationPositionSection()
{
	SeparatorText("Position");

	long long position[] = {
		TransformSettings::PanX / TransformSettings::UiPanScale,
		TransformSettings::PanY / TransformSettings::UiPanScale
	};

	bool positionModified = SliderScalarN(
		"##sliderPosition", ImGuiDataType_S64, position, std::size(position),
		&TransformSettings::MinUiPan, &TransformSettings::MaxUiPan
	);

	if (positionModified)
	{
		TransformSettings::PanX = position[0] * TransformSettings::UiPanScale;
		TransformSettings::PanY = position[1] * TransformSettings::UiPanScale;
	}
}

void ImGuiMain::RenderSimulationZoomSection()
{
	SeparatorText("Zoom");

	if (RadioButton("Mouse", TransformSettings::ZoomOnMouse))
		TransformSettings::ZoomOnMouse = true;

	SetItemTooltip("Zoom on the mouse cursor");
	SameLine();

	if (RadioButton("Center", !TransformSettings::ZoomOnMouse))
		TransformSettings::ZoomOnMouse = false;

	SetItemTooltip("Zoom on the center of the screen");

	bool zoomModified = SliderScalar(
		"##sliderZoom", ImGuiDataType_U16, &TransformSettings::Zoom,
		&TransformSettings::MinUiZoom, &TransformSettings::MaxUiZoom
	);

	if (zoomModified)
		TransformSettings::ZoomOnMouse = false;
}

void ImGuiMain::RenderPerformanceSection()
{
	using MainSettings::Gui, MainSettings::TargetFps;

	if (CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Checkbox("Show Metrics", &MainSettings::ShowMetrics);
		Checkbox("Thread Sleep", &MainSettings::ThreadSleep);
		SetItemTooltip(
			"Reduces CPU usage by sleeping as close\n"
			"to 1ms as possible every update\n"
			"Reduces max FPS and FPS stability"
		);

		Spacing();
		SeparatorText("Target FPS");

		if (InputFloat("##textTargetFps", &TargetFps, 1, 10, "%.2f"))
			Gui->SetTargetFps(TargetFps);

		if (SliderFloat("##sliderTargetFps", &TargetFps, 0, 500, "%.2f"))
			Gui->SetTargetFps(TargetFps);

		if (Button("Step Frame", ImVec2(-1, 0)))
			Gui->StepFrame();
	}
}

void ImGuiMain::RenderInterfaceSection()
{
	using MainSettings::SelectedFontSize;

	if (CollapsingHeader("Interface"))
	{
		if (Checkbox("Fullscreen", &MainSettings::FullscreenEnabled))
			MainSettings::Gui->ApplyFullscreenState();

		SeparatorText("Font Size");

		bool fontChanged = SliderInt(
			"##sliderFontSize", &SelectedFontSize, 0, 3,
			fontSizes[SelectedFontSize], ImGuiSliderFlags_NoInput
		);

		if (fontChanged)
		{
			ImGuiIO& io = GetIO();
			io.FontDefault = io.Fonts->Fonts[SelectedFontSize];
		}
	}
}
