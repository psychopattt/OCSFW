#include "ImGuiLog.h"

#include "imgui/imgui.h"
#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"

using namespace ImGui;
using MainSettings::Log;

void ImGuiLog::Render()
{
	if (Log.IsEmpty())
		return;

	HandleFullscreen();
	bool keepLog = true;
	
	if (Begin("Log", &keepLog, windowFlags))
	{
		RenderOptionsPopup("titleBarPopup");
		RenderWindowBody();
	}
	
	End();

	if (!keepLog)
		Log.Clear();

	if (wasFullscreen)
		PopStyleVar();
}

void ImGuiLog::HandleFullscreen()
{
	ImVec2 viewportSize = GetMainViewport()->WorkSize;
	bool enabledFullscreen = !wasFullscreen && isFullscreen;
	bool disabledFullscreen = wasFullscreen && !isFullscreen;

	if (enabledFullscreen)
	{
		wasFullscreen = true;
		windowFlags = ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse;
	}
	else if (disabledFullscreen)
	{
		wasFullscreen = false;
		windowFlags = ImGuiWindowFlags_None;
		SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Always);
		SetNextWindowPos(ImVec2(viewportSize.x / 2, viewportSize.y / 2),
			ImGuiCond_Always, ImVec2(0.5, 0.5));
	}

	if (isFullscreen)
	{
		PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		SetNextWindowSize(viewportSize, ImGuiCond_Always);
		SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	}
}

void ImGuiLog::RenderWindowBody()
{
	BeginChild("logScrollArea", ImVec2(0, 0), ImGuiChildFlags_None,
		ImGuiWindowFlags_HorizontalScrollbar);

	TextUnformatted(Log);

	if (autoScroll && GetScrollY() >= GetScrollMaxY())
		SetScrollHereY(1.0f);

	EndChild();
	RenderOptionsPopup("textAreaPopup");
}

void ImGuiLog::RenderOptionsPopup(const char* popupId)
{
	if (BeginPopupContextItem(popupId))
	{
		Checkbox("Fullscreen", &isFullscreen);
		Checkbox("Auto-Scroll", &autoScroll);
		float buttonWidth = (GetWindowWidth() - 24) / 2;

		if (Button("Copy", ImVec2(buttonWidth, 0)))
		{
			SetClipboardText(Log);
			CloseCurrentPopup();
			EndPopup();
			return;
		}

		SameLine();

		if (Button("Clear", ImVec2(buttonWidth, 0)))
			Log.Clear();

		EndPopup();
	}
}
