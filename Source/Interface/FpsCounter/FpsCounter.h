#pragma once

#include <string>

class FpsCounter
{
	public:
		FpsCounter(double updatesPerSecond);
		bool Update();
		double GetFps() const;
		double GetFrametime() const;
		std::string ToString() const;

	private:
		double fps = 0;
		double frametime = 0;
		double lastUpdateTime = 0;
		unsigned int frameCounter = 0;
		double updateInterval;
};
