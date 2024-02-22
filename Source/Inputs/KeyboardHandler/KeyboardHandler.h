#pragma once

class KeyboardHandler
{
	public:
		virtual void HandleKeyboard(struct GLFWwindow* window,
			int key, int scanCode, int action, int mods);
		virtual void Update(double deltaTime);

	protected:
		virtual void ApplyHideGui(int key, int action);
		virtual void ApplyRestart(int key, int action);
		virtual void ApplyFullscreen(int key, int action);
		virtual void ApplyFrameStep(int key, int action);
		virtual void ApplySpeedMultiplier();
		virtual void ApplyPan(double deltaTime);
		bool GetKey(int key);
};
