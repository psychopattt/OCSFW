#include "FpsCounter.h"
#include "GLFW/glfw3.h"

FpsCounter::FpsCounter(double updatesPerSecond)
	: updateInterval(1 / updatesPerSecond) { }

void FpsCounter::Reset()
{
	totalFrameCounter = 0;
}

bool FpsCounter::Update()
{
	totalFrameCounter++;
	intervalFrameCounter++;
	double currentTime = glfwGetTime();
	double timeDiff = currentTime - lastUpdateTime;

	if (timeDiff >= updateInterval)
	{
		frametime = 1000 * timeDiff / intervalFrameCounter;
		fps = intervalFrameCounter / timeDiff;
		lastUpdateTime = currentTime;
		intervalFrameCounter = 0;

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

unsigned long long FpsCounter::GetFrames() const
{
	return totalFrameCounter;
}

std::string FpsCounter::ToString() const
{
	char buffer[25];

	snprintf(
		buffer, std::size(buffer), "%.2ffps, "
		"%.3fms", fps, frametime
	);

	return buffer;
}
