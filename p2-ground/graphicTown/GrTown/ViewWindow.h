#pragma once

#include "FL\Fl_Gl_Window.H"
#include "FL\Fl.H"

class ViewWindow : public Fl_Gl_Window
{
public:
	ViewWindow(int x, int y, int w, int h,
		const char* l = 0);
	~ViewWindow();
	int handle(int event);
private:
	void draw();
};

