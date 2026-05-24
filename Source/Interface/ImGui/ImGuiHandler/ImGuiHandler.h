#pragma once

class ImGuiHandler
{
	public:
		ImGuiHandler(struct GLFWwindow* glfwWindow);
		void Render();
		~ImGuiHandler();

	private:
		void ApplyConfiguration();
		void ApplyColors(struct ImVec4* colors);
		void BeginRender();
		void EndRender();
};
