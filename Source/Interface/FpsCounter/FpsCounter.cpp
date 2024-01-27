#include "FpsCounter.h"
#include "GLFW/glfw3.h"

FpsCounter::FpsCounter(double updatesPerSecond)
{
	updateInterval = 1 / updatesPerSecond;
}

bool FpsCounter::Update()
{
	frameCounter++;
	double currentTime = glfwGetTime();
	double timeDiff = currentTime - lastUpdateTime;

	if (timeDiff >= updateInterval)
	{
		frametime = 1000 * timeDiff / frameCounter;
		fps = frameCounter / timeDiff;
		lastUpdateTime = currentTime;
		frameCounter = 0;

		return true;
	}

	return false;
}

double FpsCounter::GetFps() const
{
	return fps;
}

double FpsCounter::GetFrametime() const
{
	return frametime;
}

std::string FpsCounter::ToString() const
{
	char buffer[25];

	snprintf(
		buffer, std::size(buffer), "%.2ffps,"
		"%.3fms", fps, frametime
	);

	return buffer;
}
