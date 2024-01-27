#pragma once

#include "../ImGuiWindow.h"

class ImGuiLog : public ImGuiWindow
{
	public:
		void Render();

	private:
		int windowFlags = 0;
		bool autoScroll = true;
		bool isFullscreen = false;
		bool wasFullscreen = true;

		void HandleFullscreen();
		void RenderWindowBody();
		void RenderOptionsPopup(const char* popupId);
};
