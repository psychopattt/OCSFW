#include "ImGuiMetrics.h"

#include "imgui/imgui.h"

#include "Interface/Interface.h"
#include "Settings/MainSettings.h"
#include "Interface/FpsCounter/FpsCounter.h"

using namespace ImGui;

enum Position : signed char
{
	Custom = -2,
	Center = -1,
	TopLeft = 0,
	TopRight = 1,
	BottomLeft = 2,
	BottomRight = 3
};

enum DisplayFlags : unsigned char
{
	None = 0,
	Simulation = 1 << 0,
	Interface = 1 << 1,
	DearImGui = 1 << 2,
	Label = 1 << 3,
	Fps = 1 << 4,
	Frametime = 1 << 5,
	Frames = 1 << 6,
	Everything = 0xFF
};

ImGuiMetrics::ImGuiMetrics()
{
	position = TopLeft;
	metricsText.reserve(200);
	displayFlags = static_cast<DisplayFlags>(Everything ^ DearImGui ^ Frames);
	windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav;
}

void ImGuiMetrics::Render()
{
	if (!MainSettings::ShowMetrics)
		return;

	ApplyPosition();
	SetNextWindowBgAlpha(0.75f);

	if (Begin("Metrics", &MainSettings::ShowMetrics, windowFlags))
	{
		RenderMetrics();
		RenderMainMenu();
	}

	End();
}

void ImGuiMetrics::ApplyPosition()
{
	if (position == Custom)
	{
		windowFlags &= ~ImGuiWindowFlags_NoMove;
	}
	else if (position == Center)
	{
		SetNextWindowPos(GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		windowFlags |= ImGuiWindowFlags_NoMove;
	}
	else
	{
		const ImGuiViewport* viewport = GetMainViewport();
		ImVec2 workSize = viewport->WorkSize;
		ImVec2 workPos = viewport->WorkPos;

		ImVec2 windowPos = ImVec2(
			(position & 1) ? (workPos.x + workSize.x - padding) : (workPos.x + padding),
			(position & 2) ? (workPos.y + workSize.y - padding) : (workPos.y + padding)
		);

		ImVec2 windowPivot = ImVec2(
			(position & 1) ? 1.0f : 0.0f,
			(position & 2) ? 1.0f : 0.0f
		);

		SetNextWindowPos(windowPos, ImGuiCond_Always, windowPivot);
		windowFlags |= ImGuiWindowFlags_NoMove;
	}
}

void ImGuiMetrics::RenderMetrics()
{
	metricsText.clear();

	const FpsCounter* simFpsCounter = MainSettings::Gui->GetSimulationFpsCounter();
	AppendMetrics(Simulation, "Simulation", simFpsCounter);

	const FpsCounter* uiFpsCounter = MainSettings::Gui->GetInterfaceFpsCounter();
	AppendMetrics(Interface, "Interface", uiFpsCounter);

	double imGuiFps = GetIO().Framerate;
	AppendMetrics(DearImGui, "ImGui", imGuiFps, 1000 / imGuiFps, uiFpsCounter->GetFrames());

	Text(metricsText.c_str());
}

void ImGuiMetrics::AppendMetrics(DisplayFlags type, const char* label, const FpsCounter* fpsCounter)
{
	AppendMetrics(type, label, fpsCounter->GetFps(), fpsCounter->GetFrametime(), fpsCounter->GetFrames());
}

void ImGuiMetrics::AppendMetrics(DisplayFlags type, const char* label,
	double fps, double frametime, unsigned long long frames)
{
	if (displayFlags & type)
	{
		if (!metricsText.empty())
			metricsText += "\n";

		if (displayFlags & Label)
		{
			metricsText += label;
			metricsText += ": ";
		}

		if (displayFlags & Fps)
			AppendFormattedMetric(fps, "fps");

		if (displayFlags & Frametime)
		{
			if (displayFlags & Fps)
				metricsText += ", ";

			AppendFormattedMetric(frametime, "ms");
		}

		if (displayFlags & Frames)
		{
			if (displayFlags & Fps || displayFlags & Frametime)
				metricsText += ", ";

			metricsText += std::to_string(frames) + "f";
		}
	}
}

void ImGuiMetrics::AppendFormattedMetric(double metric, const char* units)
{
	char formattedMetric[11];

	snprintf(
		formattedMetric, std::size(formattedMetric),
		"%.*f", metricsDigits, metric
	);

	metricsText += formattedMetric;
	metricsText += units;
}

void ImGuiMetrics::RenderMainMenu()
{
	if (BeginPopupContextWindow())
	{
		if (BeginMenu("Display"))
			RenderDisplayMenu();

		if (BeginMenu("Position"))
			RenderPositionMenu();

		if (MenuItem("Close"))
			MainSettings::ShowMetrics = false;

		EndPopup();
	}
}

void ImGuiMetrics::RenderDisplayMenu()
{
	RenderDisplaySelectable("Simulation Metrics", Simulation);
	RenderDisplaySelectable("Interface Metrics", Interface);
	RenderDisplaySelectable("ImGui Metrics", DearImGui);

	RenderSeparator();

	RenderDisplaySelectable("Fps Metrics", Fps);
	RenderDisplaySelectable("Frametime Metrics", Frametime);
	RenderDisplaySelectable("Frames Metrics", Frames);
	RenderDisplaySelectable("Metrics Labels", Label);

	RenderSeparator();

	SetNextItemWidth(CalcItemWidth() * 0.35f);
	SliderInt("Digits", &metricsDigits, 0, 3, "%d", ImGuiSliderFlags_NoInput);
	EndMenu();
}

void ImGuiMetrics::RenderDisplaySelectable(const char* label, DisplayFlags value)
{
	if (Selectable(label, displayFlags & value, ImGuiSelectableFlags_NoAutoClosePopups))
		displayFlags = static_cast<DisplayFlags>(displayFlags ^ value);
}

void ImGuiMetrics::RenderPositionMenu()
{
	RenderPositionSelectable("Custom", Custom);
	RenderPositionSelectable("Center", Center);
	RenderPositionSelectable("Top Left", TopLeft);
	RenderPositionSelectable("Top Right", TopRight);
	RenderPositionSelectable("Bottom Left", BottomLeft);
	RenderPositionSelectable("Bottom Right", BottomRight);
	EndMenu();
}

void ImGuiMetrics::RenderPositionSelectable(const char* label, Position value)
{
	if (Selectable(label, position == value))
		position = value;
}

void ImGuiMetrics::RenderSeparator()
{
	Spacing();
	Separator();
	Spacing();
}
