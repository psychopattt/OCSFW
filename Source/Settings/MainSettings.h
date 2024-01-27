#pragma once

namespace MainSettings
{
	inline class Simulation* Sim = nullptr;
	inline bool ThreadSleep = true;
	inline float TargetFps = 100;

	inline class Interface* Gui = nullptr;
	inline bool FullscreenEnabled = false;
	inline bool ShowMetrics = false;
	inline int SelectedFontSize = 1;

	extern class LogString Log;
}
