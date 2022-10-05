#pragma once
#include <vector>
#include <imgui.h>

class IWindow
{
public:
	IWindow();
	virtual ~IWindow();
	bool operator==(const IWindow& RHS);
	virtual void Tick();
	virtual void Draw() = 0;
	bool IsEnabled() { return bEnabled; };
	bool ToggleEnabled() { bEnabled = !bEnabled; return bEnabled; }

	static void DrawGUI();
protected:
	static std::vector<IWindow*> DrawList;
private:
	size_t WindowID;
	bool bEnabled;
};