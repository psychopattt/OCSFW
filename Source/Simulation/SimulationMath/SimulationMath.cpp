#include "SimulationMath.h"

#include <cmath>

#include "Interface/Viewport/Viewport.h"
#include "Settings/TransformSettings.h"
#include "Simulation/Simulation.h"
#include "Interface/Interface.h"
#include "Settings/MainSettings.h"

namespace SimulationMath
{
	double ScaleZoom(double zoom)
	{
		return pow(1.2, zoom / TransformSettings::FastMultiplier);
	}

	double GetMaxPanAtZoom(double zoom)
	{
		// Calculate pan scale at specified zoom
		double panScale = 1.0 / ScaleZoom(zoom);

		// Multipy max pan without zoom by pan scale
		return TransformSettings::MaxPan * panScale;
	}

	double GetVisibleWorldSize(double worldSize, double screenSize, double viewportSize)
	{
		double visibleWorldSizeRatio = screenSize / viewportSize;
		double visibleWorldSize = worldSize * visibleWorldSizeRatio;

		return visibleWorldSize;
	}

	int ConvertWorldCoordToPixelCoord(double worldCoord, double textureSize)
	{
		double worldToPixelRatio = TransformSettings::MaxPan / textureSize;

		// Subtract 0.5 to return pixel center
		double pixelCoord = worldCoord / worldToPixelRatio - 0.5;

		return lround(pixelCoord);
	}

	int ConvertPixelCoordsToPixelId(int pixelX, int pixelY)
	{
		int simWidth = MainSettings::Sim->GetWidth();
		int simHeight = MainSettings::Sim->GetHeight();

		if (pixelX < 0 || pixelX >= simWidth || pixelY < 0 || pixelY >= simHeight)
			return -1;

		return pixelY * simWidth + pixelX;
	}

	void ComputeScreenCenterWorldCoords(double& screenCenterWorldX, double& screenCenterWorldY)
	{
		// Get absolute world coord [0, 1] from center relative pan [-1, 1]
		double worldSizeHalf = TransformSettings::MaxPan / 2.0;
		screenCenterWorldX = worldSizeHalf + TransformSettings::PanX;
		screenCenterWorldY = worldSizeHalf + TransformSettings::PanY;
	}

	double GetMouseWorldCoordOffsetCenter(double screenCoord, double screenSize,
		double viewportSize, double viewportScale, double worldSize)
	{
		// Get screen coord relative to center [-1, 1] from absolute screen coord
		double screenCoordRelativeCenter = 2.0 * screenCoord / screenSize - 1.0;

		// Calculate visible portion of the world size
		double visibleWorldSize = GetVisibleWorldSize(worldSize, screenSize, viewportSize);

		// Map visible world size to [-1, 1] by dividing by 2
		// Multiply visible world size by relative screen coord to get world coord
		double worldCoordCenter = visibleWorldSize / 2.0 * screenCoordRelativeCenter;

		// Ensure world coord stays valid on viewports larger than GL_MAX_VIEWPORT_DIMS
		worldCoordCenter /= viewportScale;

		// Return mouse world coordinate offset from screen center
		return worldCoordCenter;
	}

	void ComputeMouseWorldCoords(double& worldX, double& worldY)
	{
		using MainSettings::Gui;

		int interfaceWidth, interfaceHeight;
		Gui->GetSize(interfaceWidth, interfaceHeight);

		int viewportWidth, viewportHeight;
		Gui->GetViewport()->GetSize(viewportWidth, viewportHeight);

		double mouseX, mouseY;
		MainSettings::Gui->GetMousePosition(mouseX, mouseY);

		double worldSize = GetMaxPanAtZoom(TransformSettings::Zoom);

		// Get world coords of current screen center
		ComputeScreenCenterWorldCoords(worldX, worldY);

		// Add mouse world coord offset X from center
		worldX += GetMouseWorldCoordOffsetCenter(
			mouseX, interfaceWidth, viewportWidth,
			TransformSettings::ViewportScaleX, worldSize
		);

		// Subtract mouse world coord offset Y from center
		worldY -= GetMouseWorldCoordOffsetCenter(
			mouseY, interfaceHeight, viewportHeight,
			TransformSettings::ViewportScaleY, worldSize
		);
	}

	void ComputeMousePixelCoords(int& pixelX, int& pixelY)
	{
		double worldX, worldY;
		ComputeMouseWorldCoords(worldX, worldY);

		pixelX = ConvertWorldCoordToPixelCoord(worldX, MainSettings::Sim->GetWidth());
		pixelY = ConvertWorldCoordToPixelCoord(worldY, MainSettings::Sim->GetHeight());
	}
}
