#pragma once

#include <memory>

using std::unique_ptr;

class Viewport;
class FpsLimiter;
class FpsCounter;
struct GLFWwindow;
enum UpdateType : char;

class Interface
{
	public:
		Interface(int width, int height, const char* title);
		bool ShouldExit() const;
		UpdateType Update();

		void TriggerResize() const;
		void ApplyFullscreenState() const;
		void SetTargetFps(float targetFps) const;
		void StepFrame();

		void GetSize(int& width, int& height) const;
		const double* GetMetrics() const;
		class WindowTitle* GetTitle() const;
		void GetMousePosition(double& posX, double& posY) const;
		Viewport* GetViewport() const;
		GLFWwindow* GetWindow() const;

		~Interface();

	private:
		int width;
		int height;
		const int initialWidth;
		const int initialHeight;
		bool stepFrame = false;
		double metrics[4] = { };

		GLFWwindow* window;
		unique_ptr<Viewport> viewport;
		unique_ptr<class WindowTitle> title;
		unique_ptr<FpsLimiter> uiFpsLimiter;
		unique_ptr<FpsCounter> uiFpsCounter;
		unique_ptr<FpsLimiter> simFpsLimiter;
		unique_ptr<FpsCounter> simFpsCounter;
		unique_ptr<class ImGuiHandler> imGuiHandler;
		unique_ptr<class InputHandler> inputHandler;

		void SetOpenGlVersion(int major, int minor);
		void CreateWindow();
		void InitializeGlad();
		void CreateFpsHandlers();
		void ActivateCallbacks();
		void UpdateTitle() const;

		static void ErrorCallback(int error, const char* message);
		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void MouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
};
