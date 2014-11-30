#pragma once

#include <FL\Fl_Double_Window.H>
#include "ViewWindow.h"

class WindowUI : public Fl_Double_Window
{
public:
	WindowUI();
	~WindowUI();
	ViewWindow* viewWindow;
	
};

