#pragma once

#include "../ImGuiWindow.h"

class ImGuiMain : public ImGuiWindow
{
	public:
		void Initialize();
		void Render();

	private:
		int newSimSize[2] = { };
		unsigned int newSimSeed = 0;
		static constexpr unsigned int seedStepSpeed = 1;

		void RenderSimulationSection();
		void RenderSimulationSettingsSection();
		void RenderSimulationPositionSection();
		void RenderSimulationZoomSection();
		void RenderPerformanceSection();
		void RenderInterfaceSection();
};
