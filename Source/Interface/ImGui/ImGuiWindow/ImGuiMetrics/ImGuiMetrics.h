#pragma once

#include <string>

#include "../ImGuiWindow.h"

class FpsCounter;
enum Position : signed char;
enum DisplayFlags : unsigned char;

class ImGuiMetrics : public ImGuiWindow
{
	public:
		ImGuiMetrics();
		void Render() override;

	private:
		std::string metricsText;
		int metricsDigits = 2;
		Position position;
		DisplayFlags displayFlags;
		const float padding = 10.0f;
		int windowFlags;

		void ApplyPosition();
		void RenderMetrics();
		void AppendMetrics(DisplayFlags type, const char* label, const FpsCounter* fpsCounter);
		void AppendMetrics(DisplayFlags type, const char* label,
			double fps, double frametime, unsigned long long frames);
		void AppendFormattedMetric(double metric, const char* units);
		void RenderMainMenu();
		void RenderDisplayMenu();
		void RenderDisplaySelectable(const char* label, DisplayFlags value);
		void RenderPositionMenu();
		void RenderPositionSelectable(const char* label, Position value);
		void RenderSeparator();
};
