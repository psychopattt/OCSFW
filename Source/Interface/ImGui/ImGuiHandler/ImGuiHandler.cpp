#include "ImGuiHandler.h"

#include <memory>

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "ProggyRegular.cpp"
#include "../ImGuiWindow/ImGuiWindow.h"
#include "../ImGuiWindow/ImGuiLog/ImGuiLog.h"
#include "../ImGuiWindow/ImGuiMain/ImGuiMain.h"
#include "../ImGuiWindow/ImGuiDebug/ImGuiDebug.h"
#include "../ImGuiWindow/ImGuiMetrics/ImGuiMetrics.h"
#include "Settings/MainSettings.h"
#include "Settings/ImGuiMenus.h"

using std::unique_ptr, std::make_unique;

const unique_ptr<ImGuiWindow> defaultMenus[] = {
	make_unique<ImGuiDebug>(), make_unique<ImGuiMain>(),
	make_unique<ImGuiLog>(), make_unique<ImGuiMetrics>(),
};

ImGuiHandler::ImGuiHandler(GLFWwindow* glfwWindow)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = NULL; // Disable save/load
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	LoadFonts();
	ApplyStyle();

	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init();

	for (int i = 0; i < ImGuiMenus::MenuCount; i++)
		ImGuiMenus::Menus[i]->Initialize();

	for (const unique_ptr<ImGuiWindow>& menu : defaultMenus)
		menu->Initialize();
}

void ImGuiHandler::LoadFonts()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromMemoryCompressedBase85TTF(ProggyRegularData, 20);
	io.Fonts->AddFontFromMemoryCompressedBase85TTF(ProggyRegularData, 25);
	io.Fonts->AddFontFromMemoryCompressedBase85TTF(ProggyRegularData, 30);
	io.FontDefault = io.Fonts->Fonts[MainSettings::SelectedFontSize];
}

void ImGuiHandler::ApplyStyle()
{
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding = ImVec2(4, 4);
	style.WindowRounding = 3;

	const ImVec4 activeGreen = ImVec4(0.16f, 0.43f, 0.07f, 1.00f);
	const ImVec4 hoveredGreen = ImVec4(0.16f, 0.43f, 0.07f, 0.80f);
	const ImVec4 backgroundGreen = ImVec4(0.16f, 0.43f, 0.07f, 0.60f);

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.60f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.35f, 0.35f, 0.35f, 0.70f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.40f, 0.40f, 0.40f, 0.80f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.75f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.36f, 0.36f, 0.36f, 0.54f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 1.00f, 0.00f, 0.50f);
	colors[ImGuiCol_CheckMark] = backgroundGreen;
	colors[ImGuiCol_SliderGrab] = hoveredGreen;
	colors[ImGuiCol_SliderGrabActive] = activeGreen;
	colors[ImGuiCol_Button] = backgroundGreen;
	colors[ImGuiCol_ButtonHovered] = hoveredGreen;
	colors[ImGuiCol_ButtonActive] = activeGreen;
	colors[ImGuiCol_Header] = backgroundGreen;
	colors[ImGuiCol_HeaderHovered] = hoveredGreen;
	colors[ImGuiCol_HeaderActive] = activeGreen;
	colors[ImGuiCol_Separator] = backgroundGreen;
	colors[ImGuiCol_SeparatorHovered] = hoveredGreen;
	colors[ImGuiCol_SeparatorActive] = activeGreen;
	colors[ImGuiCol_ResizeGrip] = backgroundGreen;
	colors[ImGuiCol_ResizeGripHovered] = hoveredGreen;
	colors[ImGuiCol_ResizeGripActive] = activeGreen;
	colors[ImGuiCol_Tab] = backgroundGreen;
	colors[ImGuiCol_TabHovered] = hoveredGreen;
	colors[ImGuiCol_TabActive] = activeGreen;
}

void ImGuiHandler::BeginRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiHandler::Render()
{
	BeginRender();

	if (!MainSettings::HideGui)
	{
		for (int i = 0; i < ImGuiMenus::MenuCount; i++)
			ImGuiMenus::Menus[i]->Render();

		for (const unique_ptr<ImGuiWindow>& menu : defaultMenus)
			menu->Render();
	}

	EndRender();
}

void ImGuiHandler::EndRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiHandler::~ImGuiHandler()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
