#include "Interface.h"

#include <chrono>
#include <thread>
#include <string>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"
#include "Viewport/Viewport.h"
#include "Settings/UpdateType.h"
#include "Inputs/InputHandler.h"
#include "FpsLimiter/FpsLimiter.h"
#include "FpsCounter/FpsCounter.h"
#include "WindowTitle/WindowTitle.h"
#include "ImGui/ImGuiHandler/ImGuiHandler.h"

using std::make_unique;

Interface::Interface(int width, int height, const char* title) :
	width(width), height(height), initialWidth(width),
	initialHeight(height)
{
	this->title = make_unique<WindowTitle>(title);

	glfwInit();
	SetOpenGlVersion(4, 6);
	CreateWindow();
	InitializeGlad();
	ActivateCallbacks();
	CreateFpsHandlers();

	MainSettings::Gui = this;
	viewport = make_unique<Viewport>();
	inputHandler = make_unique<InputHandler>();
	imGuiHandler = make_unique<ImGuiHandler>(window);

	// Create the OpenGL window
	ResizeCallback(window, width, height);
}

void Interface::SetOpenGlVersion(int major, int minor)
{
	// Set OpenGL major (x.) and minor (.x) version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

	// Use core (non-deprecated) OpenGL functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Interface::CreateWindow()
{
	// Create a centered GLFW window
	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_POSITION_X, (videoMode->width / 2) - (width / 2));
	glfwWindowHint(GLFW_POSITION_Y, (videoMode->height / 2) - (height / 2));
	window = glfwCreateWindow(width, height, title->ToString().c_str(), NULL, NULL);

	if (window == NULL)
	{
		MainSettings::Log << "GLFW Error - Failed to create window" << "\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Disable V-Sync
}

void Interface::InitializeGlad()
{
	// Load GLAD and let it configure OpenGL
	int openGlVersion = gladLoadGL(glfwGetProcAddress);

	if (openGlVersion == 0)
		MainSettings::Log << "GLAD Error - Failed to initialize OpenGL" << "\n";
}

void Interface::CreateFpsHandlers()
{
	MainSettings::TargetFps = fmaxf(100.0f, static_cast<float>(
		glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate
	));

	simFpsLimiter = make_unique<FpsLimiter>(MainSettings::TargetFps);
	uiFpsLimiter = make_unique<FpsLimiter>(MainSettings::TargetFps);
	simFpsCounter = make_unique<FpsCounter>(10);
	uiFpsCounter = make_unique<FpsCounter>(2);
}

bool Interface::ShouldExit() const
{
	return glfwWindowShouldClose(window);
}

UpdateType Interface::Update()
{
	UpdateType updateType = None;

	if (MainSettings::ThreadSleep)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	if (simFpsLimiter->Update() || stepFrame)
	{
		stepFrame = false;
		updateType |= SimulationUpdate;

		if (simFpsCounter->Update())
			title->SetSubText("Simulation: " + simFpsCounter->ToString());
	}

	if (uiFpsLimiter->Update())
	{
		updateType |= DisplayUpdate;

		if (uiFpsCounter->Update())
			title->GetSubTitle()->SetSubText("Interface: " + uiFpsCounter->ToString());

		UpdateTitle();
		glfwPollEvents();
		inputHandler->Update();
		imGuiHandler->Render();
		glfwSwapBuffers(window);
	}

	return updateType;
}

void Interface::UpdateTitle() const
{
	if (title->IsOutdated())
	{
		title->Update();
		glfwSetWindowTitle(window, title->ToString().c_str());
	}
}

void Interface::NotifyRestart() const
{
	ResizeCallback(window, width, height);
	simFpsCounter->Reset();
}

void Interface::ApplyFullscreenState() const
{
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

	if (MainSettings::FullscreenEnabled)
	{
		glfwSetWindowMonitor(
			window, primaryMonitor, 0, 0, videoMode->width,
			videoMode->height, GLFW_DONT_CARE
		);
	}
	else
	{
		glfwSetWindowMonitor(
			window, NULL, (videoMode->width / 2) - (initialWidth / 2),
			(videoMode->height / 2) - (initialHeight / 2), initialWidth,
			initialHeight, GLFW_DONT_CARE
		);
	}
}

void Interface::SetTargetFps(float targetFps) const
{
	using MainSettings::TargetFps;

	TargetFps = targetFps < 0 ? 0 : targetFps;
	MainSettings::ThreadSleep = TargetFps < 100;
	simFpsLimiter->SetTargetFps(TargetFps);
}

void Interface::StepFrame()
{
	if (MainSettings::TargetFps > 0)
		SetTargetFps(0);

	stepFrame = true;
}

WindowTitle* Interface::GetTitle() const
{
	return title.get();
}

void Interface::GetSize(int& width, int& height) const
{
	width = this->width;
	height = this->height;
}

const FpsCounter* Interface::GetInterfaceFpsCounter() const
{
	return uiFpsCounter.get();
}

const FpsCounter* Interface::GetSimulationFpsCounter() const
{
	return simFpsCounter.get();
}

void Interface::GetMousePosition(double& posX, double& posY) const
{
	glfwGetCursorPos(window, &posX, &posY);
}

Viewport* Interface::GetViewport() const
{
	return viewport.get();
}

GLFWwindow* Interface::GetWindow() const
{
	return window;
}

Interface::~Interface()
{
	imGuiHandler.reset();
	glfwDestroyWindow(window);
	glfwTerminate();
}
