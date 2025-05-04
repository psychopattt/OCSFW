#include "SimulationTransforms.h"

#include <cmath>

#include "Simulation/SimulationMath/SimulationMath.h"
#include "Interface/Viewport/Viewport.h"
#include "Settings/TransformSettings.h"
#include "Interface/Interface.h"
#include "Settings/MainSettings.h"

SimulationTransforms::SimulationTransforms(double* quadVertices, size_t quadVertexCount) :
	quadVertices(quadVertices),	quadVertexCount(quadVertexCount)
{
	lastZoom = TransformSettings::Zoom;
	TransformSettings::ViewportSizeChanged = true;

	auto initialQuadVertices = std::make_unique_for_overwrite<double[]>(quadVertexCount);
	std::copy(quadVertices, quadVertices + quadVertexCount, initialQuadVertices.get());
	this->initialQuadVertices = std::move(initialQuadVertices);
}

bool SimulationTransforms::ApplyTransforms()
{
	bool updatedTransforms = ApplyPanOffset();
	updatedTransforms = ApplyZoomPan() || updatedTransforms;
	updatedTransforms = ApplyMousePan() || updatedTransforms;
	updatedTransforms = ApplyZoom() || updatedTransforms;
	updatedTransforms = ApplyPan() || updatedTransforms;

	return updatedTransforms;
}

bool SimulationTransforms::ApplyPanOffset()
{
	using TransformSettings::PanOffsetX, TransformSettings::PanOffsetY;

	if (PanOffsetX == 0 && PanOffsetY == 0)
		return false;

	TransformSettings::PanX += ComputePanOffsetAxis(PanOffsetX,
		TransformSettings::PanAspectMultiplierX);
	TransformSettings::PanY += ComputePanOffsetAxis(PanOffsetY,
		TransformSettings::PanAspectMultiplierY);

	return true;
}

long long SimulationTransforms::ComputePanOffsetAxis(
	long long& panOffset, double aspectRatioMultiplier)
{
	using TransformSettings::Zoom, SimulationMath::GetPanScaleAtZoom;

	if (panOffset == 0)
		return 0;

	// Scale pan offset according to current zoom
	long long scaledPanOffset = llround(panOffset * GetPanScaleAtZoom(Zoom));

	// Keep pan speed consistent on all axes
	scaledPanOffset = llround(scaledPanOffset * aspectRatioMultiplier);

	// Ensure the scaled pan offset is at least 1
	if (scaledPanOffset == 0)
		scaledPanOffset = panOffset < 0 ? -1 : 1;

	// Reset pan offset
	panOffset = 0;

	return scaledPanOffset;
}

bool SimulationTransforms::ApplyZoomPan()
{
	using TransformSettings::Zoom, SimulationMath::GetMaxPanAtZoom,
		SimulationMath::GetMouseWorldCoordOffsetCenter, MainSettings::Gui;

	if (!TransformSettings::ZoomOnMouse || lastZoom == Zoom)
		return false;

	int interfaceWidth, interfaceHeight;
	Gui->GetSize(interfaceWidth, interfaceHeight);

	int viewportWidth, viewportHeight;
	Gui->GetViewport()->GetSize(viewportWidth, viewportHeight);

	double mouseX, mouseY;
	Gui->GetMousePosition(mouseX, mouseY);

	// Use max pan diff as world size to obtain mouse world coord diff
	double zoomMaxPanDiff = GetMaxPanAtZoom(lastZoom) - GetMaxPanAtZoom(Zoom);

	TransformSettings::PanX += llround(GetMouseWorldCoordOffsetCenter(
		mouseX, interfaceWidth, viewportWidth,
		TransformSettings::ViewportScaleX, zoomMaxPanDiff
	));

	TransformSettings::PanY -= llround(GetMouseWorldCoordOffsetCenter(
		mouseY, interfaceHeight, viewportHeight,
		TransformSettings::ViewportScaleY, zoomMaxPanDiff
	));

	return true;
}

bool SimulationTransforms::ApplyMousePan()
{
	using MainSettings::Gui;

	if (!TransformSettings::MousePanEnabled)
		return false;

	int interfaceWidth, interfaceHeight;
	Gui->GetSize(interfaceWidth, interfaceHeight);

	int viewportWidth, viewportHeight;
	Gui->GetViewport()->GetSize(viewportWidth, viewportHeight);

	double mouseX, mouseY;
	Gui->GetMousePosition(mouseX, mouseY);

	double zoomMaxPan = SimulationMath::GetMaxPanAtZoom(TransformSettings::Zoom);

	TransformSettings::PanX -= ComputeMousePanAxis(TransformSettings::MousePanStartX, mouseX,
		interfaceWidth, viewportWidth, TransformSettings::ViewportScaleX, zoomMaxPan);
	TransformSettings::PanY += ComputeMousePanAxis(TransformSettings::MousePanStartY, mouseY,
		interfaceHeight, viewportHeight, TransformSettings::ViewportScaleY, zoomMaxPan);

	return true;
}

long long SimulationTransforms::ComputeMousePanAxis(double& lastScreenCoord,
	double currentScreenCoord, double screenSize, double viewportSize,
	double viewportScale, double worldSize)
{
	// Calculate screen coord difference and update last coord
	double screenCoordDiff = currentScreenCoord - lastScreenCoord;
	lastScreenCoord = currentScreenCoord;

	// Calculate screen coord diff relative to screen size
	double screenCoordDiffRelative = screenCoordDiff / screenSize;

	// Calculate visible portion of the world size
	double visibleWorldSize = SimulationMath::GetVisibleWorldSize(
		worldSize, screenSize, viewportSize
	);

	// Multiply visible world size by relative screen coord diff to get world coord diff
	double worldCoordDiff = visibleWorldSize * screenCoordDiffRelative;

	// Ensure world diff stays valid on viewports larger than GL_MAX_VIEWPORT_DIMS
	worldCoordDiff /= viewportScale;

	return llround(worldCoordDiff);
}

bool SimulationTransforms::ApplyZoom()
{
	using TransformSettings::Zoom, TransformSettings::ViewportSizeChanged;

	if (lastZoom == Zoom && !ViewportSizeChanged)
		return false;

	double scaledZoom = SimulationMath::ScaleZoom(Zoom);

	// Ensure pixels are square on viewports larger than GL_MAX_VIEWPORT_DIMS
	double scaleX = scaledZoom * TransformSettings::ViewportScaleX;
	double scaleY = scaledZoom * TransformSettings::ViewportScaleY;

	for (size_t i = 0; i < quadVertexCount; i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] * scaleX;
		quadVertices[i + 1] = initialQuadVertices[i + 1] * scaleY;
	}

	lastZoom = Zoom;
	ViewportSizeChanged = false;

	return true;
}

bool SimulationTransforms::ApplyPan()
{
	using TransformSettings::PanX, TransformSettings::PanY;

	if (lastPanX == PanX && lastPanY == PanY)
		return false;

	double panX = ComputePanAxis(PanX);
	double panY = ComputePanAxis(PanY);

	// Apply new vertex coordinates
	for (size_t i = 2; i < quadVertexCount; i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] + panX;
		quadVertices[i + 1] = initialQuadVertices[i + 1] + panY;
	}

	lastPanX = PanX;
	lastPanY = PanY;

	return true;
}

double SimulationTransforms::ComputePanAxis(long long& pan)
{
	using TransformSettings::MaxPan;

	// Loop pan to opposite side if min or max is reached
	if (pan < -MaxPan || pan > MaxPan)
		pan += (pan < 0 ? 2 : -2) * MaxPan;

	// Convert pan to vertex coordinates
	return static_cast<double>(pan) / MaxPan;
}
