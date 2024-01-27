#pragma once

#include <string>
#include "../ImGuiWindow.h"

enum Position : signed char;
enum DisplayFlags : unsigned char;

class ImGuiMetrics : public ImGuiWindow
{
	public:
		ImGuiMetrics();
		void Render();

	private:
		int windowFlags;
		Position position;
		int metricsDigits = 2;
		std::string metricsText;
		DisplayFlags displayFlags;
		const float padding = 10.0f;

		void ApplyPosition();
		void RenderMetrics();
		void AppendMetricsType(DisplayFlags type,
			const char* label, const double* metrics);
		void AppendFormattedMetric(double metric, const char* units);
		void RenderMainMenu();
		void RenderDisplayMenu();
		void RenderDisplaySelectable(const char* label, DisplayFlags value);
		void RenderPositionMenu();
		void RenderPositionSelectable(const char* label, Position value);
};
