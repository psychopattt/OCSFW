#include "FpsLimiter.h"
#include "GLFW/glfw3.h"

FpsLimiter::FpsLimiter() : FpsLimiter(0) { }

FpsLimiter::FpsLimiter(double targetFps)
{
	SetTargetFps(targetFps);
	lastUpdateTime = glfwGetTime() - targetFrametime;
}

void FpsLimiter::SetTargetFps(double targetFps)
{
	paused = targetFps < 0.01;
	disabled = targetFps > 999999.0;

	if (!paused && !disabled)
		targetFrametime = 1 / targetFps;
}

bool FpsLimiter::Update()
{
	if (disabled)
		return true;
	else if (paused)
		return false;

	double currentTime = glfwGetTime();
	double timeDiff = currentTime - lastUpdateTime;

	if (timeDiff >= targetFrametime)
	{
		lastUpdateTime = currentTime;
		return true;
	}

	return false;
}
