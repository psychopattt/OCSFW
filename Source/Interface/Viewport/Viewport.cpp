#include "Viewport.h"

#include <cmath>

#include "glad/gl.h"

#include "Settings/TransformSettings.h"
#include "Simulation/Simulation.h"
#include "Interface/Interface.h"
#include "Settings/MainSettings.h"

void Viewport::ComputeSettings()
{
	int guiWidth, guiHeight;
	MainSettings::Gui->GetSize(guiWidth, guiHeight);

	int simWidth = MainSettings::Sim->GetWidth();
	int simHeight = MainSettings::Sim->GetHeight();
	double simAspectRatio = static_cast<double>(simWidth) / simHeight;

	int maxViewportSize[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewportSize);

	int viewportWidth = guiWidth, widthOffset = 0;
	int viewportHeight = guiHeight, heightOffset = 0;

	if (guiWidth > guiHeight * simAspectRatio)
	{
		TransformSettings::ViewportScaleX = 1.0;
		TransformSettings::PanAspectMultiplierX = 1.0;
		TransformSettings::PanAspectMultiplierY = simAspectRatio;
		ComputeSize(viewportHeight, heightOffset, TransformSettings::ViewportScaleY,
			simHeight, simWidth, guiHeight, guiWidth, maxViewportSize[1]);
	}
	else
	{
		TransformSettings::ViewportScaleY = 1.0;
		TransformSettings::PanAspectMultiplierY = 1.0;
		TransformSettings::PanAspectMultiplierX = static_cast<double>(simHeight) / simWidth;
		ComputeSize(viewportWidth, widthOffset, TransformSettings::ViewportScaleX,
			simWidth, simHeight, guiWidth, guiHeight, maxViewportSize[0]);
	}

	glViewport(widthOffset, heightOffset, viewportWidth, viewportHeight);
	TransformSettings::ViewportSizeChanged = true;
}

void Viewport::ComputeSize(int& viewportSize, int& sizeOffset, double& viewportScale,
	int simSize1, int simSize2, int windowSize1, int windowSize2, int maxViewportSize)
{
	// Calculate size required to make the other dimension match the window
	viewportSize = lround(simSize1 * static_cast<double>(windowSize2) / simSize2);

	if (viewportSize > maxViewportSize)
	{
		// Calculate ratio required to scale max size to desired size
		viewportScale = static_cast<double>(viewportSize) / maxViewportSize;
		viewportSize = maxViewportSize;
	}
	else
	{
		viewportScale = 1.0;
	}

	// Center viewport by moving left by half the size
	sizeOffset = -lround((viewportSize - windowSize1) / 2.0f);
}

void Viewport::GetSize(int& width, int& height) const
{
	int viewportSettings[4];
	glGetIntegerv(GL_VIEWPORT, viewportSettings);
	width = viewportSettings[2];
	height = viewportSettings[3];
}
