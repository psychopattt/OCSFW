#pragma once

class ImGuiHandler
{
	public:
		ImGuiHandler(struct GLFWwindow* glfwWindow);
		void Render();
		~ImGuiHandler();

	private:
		void LoadFonts();
		void ApplyStyle();
		void BeginRender();
		void EndRender();
};
