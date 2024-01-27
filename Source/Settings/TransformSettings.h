#pragma once

namespace TransformSettings
{
	inline constexpr char SlowMultiplier = 1;
	inline constexpr char DefaultMultiplier = 10;
	inline constexpr char FastMultiplier = 100;
	inline char SpeedMultiplier = DefaultMultiplier;

	inline bool ZoomOnMouse = true;
	inline unsigned short Zoom = 0;
	inline constexpr unsigned short MaxZoom = 0xFFFF - FastMultiplier;
	inline constexpr unsigned short MaxUiZoom = 4000;
	inline constexpr unsigned short MinUiZoom = 0;

	inline double ViewportScaleX = 1.0;
	inline double ViewportScaleY = 1.0;
	inline bool ViewportSizeChanged = false;

	inline double MousePanStartX = 0.0;
	inline double MousePanStartY = 0.0;
	inline bool MousePanEnabled = false;
	inline bool DraggingLeftClick = false;
	inline bool DraggingRightClick = false;

	inline long long PanX = 0;
	inline long long PanY = 0;
	inline long long PanOffsetX = 0;
	inline long long PanOffsetY = 0;
	inline double PanAspectMultiplierX = 1.0;
	inline double PanAspectMultiplierY = 1.0;
	inline constexpr long long PanMultiplier = 150000000000000;
	inline constexpr long long MaxPan = 10000000000000000;
	inline constexpr long long UiPanScale = MaxPan / 10000000000;
	inline constexpr long long MaxUiPan = MaxPan / UiPanScale / 2;
	inline constexpr long long MinUiPan = -MaxUiPan;
}
