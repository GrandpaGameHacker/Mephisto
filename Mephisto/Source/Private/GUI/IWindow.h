#pragma once
#include <vector>
#include <imgui.h>
class IWindow
{
public:
	IWindow() : bVisible(false) {}
	virtual ~IWindow() {};
	virtual void Draw() = 0;
	bool IsVisible() { return bVisible; };
	bool ToggleVisible() { bVisible = !bVisible; return bVisible; }
	
private:
	bool bVisible;
	static std::vector<IWindow> Windows;
};