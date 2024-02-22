#pragma once

#include "../ImGuiWindow.h"

class ImGuiDebug : public ImGuiWindow
{
	public:
		void Render() override;

#if DEBUG

	private:
		bool showImGuiDemo = false;

		void LogVersions();

#endif
};
