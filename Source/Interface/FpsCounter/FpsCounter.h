#pragma once

#include <string>

class FpsCounter
{
	public:
		FpsCounter(double updatesPerSecond);
		void Reset();
		bool Update();
		double GetFps() const;
		double GetFrametime() const;
		unsigned long long GetFrames() const;
		std::string ToString() const;

	private:
		double fps = 0;
		double frametime = 0;
		double lastUpdateTime = 0;
		unsigned int intervalFrameCounter = 0;
		unsigned long long totalFrameCounter = 0;
		const double updateInterval;
};
