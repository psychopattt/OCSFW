#pragma once

#include "../ImGuiWindow.h"

class ImGuiMain : public ImGuiWindow
{
	public:
		void Initialize() override;
		void Render() override;

	private:
		static constexpr unsigned int seedStepSpeed = 1;

		void RenderSimulationSection();
		void RenderSimulationCopyMenu();
		void RenderSimulationCopyButton();
		void RenderSimulationPasteButton();
		void RenderSimulationSettingsSection();
		void RenderSimulationPositionSection();
		void RenderSimulationZoomSection();
		void RenderPerformanceSection();
		void RenderInterfaceSection();
};
