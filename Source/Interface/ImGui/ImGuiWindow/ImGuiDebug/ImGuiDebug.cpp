#include "ImGuiDebug.h"

#if DEBUG

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"
#include "Shaders/ShaderProvider/ShaderProvider.h"

using namespace ImGui;

void ImGuiDebug::Render()
{
	SetNextWindowSize(ImVec2(200, -1), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(GetMainViewport()->WorkSize.x - 10, 10),
		ImGuiCond_FirstUseEver, ImVec2(1, 0));

	if (Begin("Debug"))
	{
		if (Button("Pack Shaders", ImVec2(-1, 0)))
			ShaderProvider::PackShaders();

		SetItemTooltip("Generate PackedShaders.cpp");

		if (Button("Log Versions", ImVec2(-1, 0)))
			LogVersions();

		Checkbox("Show ImGui Demo", &showImGuiDemo);
	}

	End();

	if (showImGuiDemo)
		ShowDemoWindow();
}

void ImGuiDebug::LogVersions()
{
	using MainSettings::Log;

	Log << "GLFW: " << glfwGetVersionString() << "\n";
	Log << "OpenGL: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << "\n";
	Log << "Dear ImGui: " << GetVersion() << "\n";
	Log << "GLAD: " << GLAD_GENERATOR_VERSION << "\n";
	Log << "\n";
}

#else

void ImGuiDebug::Render() { }

#endif
