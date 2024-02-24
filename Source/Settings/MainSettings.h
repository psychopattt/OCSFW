#pragma once

namespace MainSettings
{
	inline class Simulation* Sim = nullptr;
	inline bool ThreadSleep = true;
	inline float TargetFps = 100;
	inline unsigned int PendingSimSeed = 0;
	inline int PendingSimSize[2] = { 0, 0 };

	inline class Interface* Gui = nullptr;
	inline bool FullscreenEnabled = false;
	inline bool ShowMetrics = false;
	inline int SelectedFontSize = 1;
	inline bool HideGui = false;

	extern class LogString Log;
}
