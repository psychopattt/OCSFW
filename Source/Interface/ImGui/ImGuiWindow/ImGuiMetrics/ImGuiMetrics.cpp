#include "ImGuiMetrics.h"

#include "imgui/imgui.h"

#include "Interface/Interface.h"
#include "Settings/MainSettings.h"

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
	Everything = 0xFF
};

ImGuiMetrics::ImGuiMetrics()
{
	position = TopLeft;
	metricsText.reserve(100);
	displayFlags = static_cast<DisplayFlags>(Everything ^ DearImGui);
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
		SetNextWindowPos(GetMainViewport()->GetCenter(),
			ImGuiCond_Always, ImVec2(0.5f, 0.5f));
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
	const double* metrics = MainSettings::Gui->GetMetrics();

	AppendMetricsType(Simulation, "Simulation: ", metrics);
	AppendMetricsType(Interface, "Interface: ", metrics + 2);

	double imGuiFps = GetIO().Framerate;
	double imGuiMetrics[] = { imGuiFps, 1000.0 / imGuiFps };
	AppendMetricsType(DearImGui, "ImGui: ", imGuiMetrics);

	Text(metricsText.c_str());
}

void ImGuiMetrics::AppendMetricsType(DisplayFlags type,
	const char* label, const double* metrics)
{
	if (displayFlags & type)
	{
		if (!metricsText.empty())
			metricsText += "\n";

		if (displayFlags & Label)
			metricsText += label;

		if (displayFlags & Fps)
			AppendFormattedMetric(metrics[0], "fps");

		if (displayFlags & Frametime)
		{
			if (displayFlags & Fps)
				metricsText += ", ";

			AppendFormattedMetric(metrics[1], "ms");
		}
	}
}

void ImGuiMetrics::AppendFormattedMetric(double metric, const char* units)
{
	char formattedMetric[10];

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
	RenderDisplaySelectable("Fps Metrics", Fps);
	RenderDisplaySelectable("Frametime Metrics", Frametime);
	RenderDisplaySelectable("Metrics Labels", Label);

	Spacing();
	Separator();
	Spacing();

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
