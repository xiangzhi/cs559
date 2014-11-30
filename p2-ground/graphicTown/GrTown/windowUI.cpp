
#include "WindowUI.h"


WindowUI::WindowUI() : Fl_Double_Window(100, 50, 800, 600, "UI")
{
	viewWindow = new ViewWindow(5, 5, 590, 590);
}





WindowUI::~WindowUI()
{
}
