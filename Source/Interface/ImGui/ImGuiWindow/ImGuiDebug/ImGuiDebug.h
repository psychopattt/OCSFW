#pragma once

#include "../ImGuiWindow.h"

class ImGuiDebug : public ImGuiWindow
{
	public:
		void Render();

#if DEBUG

	private:
		bool showImGuiDemo = false;

		void LogVersions();

#endif
};
