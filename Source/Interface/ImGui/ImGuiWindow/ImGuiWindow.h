#pragma once

class ImGuiWindow
{
	public:
		virtual void Initialize() { };
		virtual void Render() = 0;
		virtual ~ImGuiWindow() = default;
};
