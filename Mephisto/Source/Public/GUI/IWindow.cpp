#include "GUI/IWindow.h"
std::vector<IWindow*> IWindow::DrawList;

IWindow::IWindow() : bEnabled(false)
{
	static size_t WindowIDCount = 0;
	WindowID = WindowIDCount++;
	IWindow::DrawList.push_back(this);
}

IWindow::~IWindow()
{
	for (size_t i = 0; i < IWindow::DrawList.size(); i++)
	{
		if (IWindow::DrawList[i] == this)
		{
			IWindow::DrawList.erase(IWindow::DrawList.begin() + i);
		};
	}
}

bool IWindow::operator==(const IWindow& RHS)
{
	return WindowID == RHS.WindowID;
}

void IWindow::Tick()
{
	if (bEnabled) {
		Draw();
	}
}

void IWindow::DrawGUI()
{
	for (auto& window : DrawList) {
		window->Tick();
	}
}

