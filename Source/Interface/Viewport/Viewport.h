#pragma once

class Viewport
{
	public:
		void ComputeSettings();
		void GetSize(int& width, int& height) const;

	private:
		void ComputeSize(int& viewportSize, int& sizeOffset, double& viewportScale,
			int simSize1, int simSize2, int windowSize1, int windowSize2, int maxViewportSize);
};
