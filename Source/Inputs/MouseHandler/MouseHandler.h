#pragma once

struct GLFWwindow;

class MouseHandler
{
	public:
		virtual void HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY);
		virtual void HandleMouseButton(GLFWwindow* window, int button, int action, int mods);
		virtual void Update(double deltaTime);

	protected:
		virtual void ApplyZoom(bool scrolledUp, bool scrolledDown);
		virtual void ApplyDragStatus();
		virtual void ApplyMousePan();
		bool GetButton(int button);
};
