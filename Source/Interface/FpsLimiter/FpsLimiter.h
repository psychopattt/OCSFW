#pragma once

class FpsLimiter
{
	public:
		FpsLimiter();
		FpsLimiter(double targetFps);
		void SetTargetFps(double targetFps);
		bool Update();

	private:
		bool paused;
		bool disabled;
		double lastUpdateTime;
		double targetFrametime;
};
