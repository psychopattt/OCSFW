#include "ImGuiMain.h"

#include "imgui/imgui.h"

#include "Settings/TransformSettings.h"
#include "Simulation/Simulation.h"
#include "Interface/Interface.h"
#include "Settings/MainSettings.h"

using namespace ImGui;

static const char* fontSizes[] = {
	"Small", "Medium", "Large", "Very Large"
};

void ImGuiMain::Initialize()
{
	newSimSeed = MainSettings::Sim->GetSeed();
	newSimSize[0] = MainSettings::Sim->GetWidth();
	newSimSize[1] = MainSettings::Sim->GetHeight();
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
		RenderSimulationSettingsSection();
		RenderSimulationPositionSection();
		RenderSimulationZoomSection();
	}
}

void ImGuiMain::RenderSimulationSettingsSection()
{
	if (Button("Restart", ImVec2(-1, 0)))
	{
		MainSettings::Sim->Restart(newSimSize[0], newSimSize[1], newSimSeed);
		MainSettings::Gui->TriggerResize();
	}

	SeparatorText("Size");
	DragInt2("##dragSize", newSimSize, 1.0f, 0, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

	SeparatorText("Seed");
	InputScalar("##textSeed", ImGuiDataType_U32, &newSimSeed, &seedStepSpeed);
}

void ImGuiMain::RenderSimulationPositionSection()
{
	SeparatorText("Position");
	long long position[] = {
		TransformSettings::PanX / TransformSettings::UiPanScale,
		TransformSettings::PanY / TransformSettings::UiPanScale
	};

	if (SliderScalarN("##sliderPosition", ImGuiDataType_S64, position,
		static_cast<int>(std::size(position)), &TransformSettings::MinUiPan,
		&TransformSettings::MaxUiPan))
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

	if (SliderScalar("##sliderZoom", ImGuiDataType_U16,
		&TransformSettings::Zoom, &TransformSettings::MinUiZoom,
		&TransformSettings::MaxUiZoom))
	{
		TransformSettings::ZoomOnMouse = false;
	}
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
