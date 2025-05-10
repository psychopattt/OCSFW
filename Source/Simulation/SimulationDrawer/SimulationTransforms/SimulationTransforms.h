#pragma once

#include <memory>

class SimulationTransforms
{
	public:
		SimulationTransforms(double* quadVertices, size_t quadVertexCount);
		bool ApplyTransforms();

	private:
		double* const quadVertices;
		const size_t quadVertexCount;
		std::unique_ptr<const double[]> initialQuadVertices;

		long long lastPanX = 0;
		long long lastPanY = 0;
		unsigned short lastZoom = 0;

		bool ComputePanOffset();
		long long ComputePanOffsetAxis(long long& panOffset,
			double aspectRatioMultiplier);

		bool ComputeZoomPan();

		bool ComputeMousePan();
		long long ComputeMousePanAxis(double& lastScreenCoord,
			double currentScreenCoord, double screenSize, double viewportSize,
			double viewportScale, double worldSize);

		bool ComputeZoom();

		bool ComputePan();
		double ComputePanAxis(long long& pan);

		void ApplyZoom();
		void ApplyPan();
};
