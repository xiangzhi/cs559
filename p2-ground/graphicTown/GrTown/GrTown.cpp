// GrTown.cpp : Defines the entry point for the console application.
//

#include <FL\Fl.H>
#include "WindowUI.h"

int main(int argc, char* argv[])
{

	WindowUI ui;

	ui.show();

	Fl::run();
	return 0;
}

