// CS559 Train Project -
// Train Window class implementation
// - note: this is code that a student might want to modify for their project
//   see the header file for more details
// - look for the TODO: in this file
// - also, look at the "TrainView" - its the actual OpenGL window that
//   we draw into
//
// Written by Mike Gleicher, October 2008
//

#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"

#include <FL/fl.h>
#include <FL/Fl_Box.h>

// for using the real time clock
#include <time.h>



/////////////////////////////////////////////////////
TrainWindow::TrainWindow(const int x, const int y) : Fl_Double_Window(x,y,800,600,"Train and Roller Coaster")
{
	// make all of the widgets
	begin();	// add to this widget
	{
		int pty=5;			// where the last widgets were drawn

		trainView = new TrainView(5,5,590,590);
		trainView->tw = this;
		trainView->world = &world;
		this->resizable(trainView);

		// to make resizing work better, put all the widgets in a group
		widgets = new Fl_Group(600,5,190,590);
		widgets->begin();

		runButton = new Fl_Button(605,pty,60,20,"Run");
		togglify(runButton);

		Fl_Button* fb = new Fl_Button(700,pty,25,20,"@>>");
		fb->callback((Fl_Callback*)forwCB,this);
		Fl_Button* rb = new Fl_Button(670,pty,25,20,"@<<");
		rb->callback((Fl_Callback*)backCB,this);
		
		arcLength = new Fl_Button(730,pty,65,20,"ArcLength");
		togglify(arcLength,1);
  
		pty+=25;
		speed = new Fl_Value_Slider(655,pty,140,20,"speed");
		speed->range(0,5);
		speed->value(2);
		speed->align(FL_ALIGN_LEFT);
		speed->type(FL_HORIZONTAL);

		pty += 30;

		// camera buttons - in a radio button group
		Fl_Group* camGroup = new Fl_Group(600,pty,195,20);
		camGroup->begin();
		worldCam = new Fl_Button(605, pty, 60, 20, "World");
        worldCam->type(FL_RADIO_BUTTON);		// radio button
        worldCam->value(1);			// turned on
        worldCam->selection_color((Fl_Color)3); // yellow when pressed
		worldCam->callback((Fl_Callback*)damageCB,this);
		trainCam = new Fl_Button(670, pty, 60, 20, "Train");
        trainCam->type(FL_RADIO_BUTTON);
        trainCam->value(0);
        trainCam->selection_color((Fl_Color)3);
		trainCam->callback((Fl_Callback*)damageCB,this);
		topCam = new Fl_Button(735, pty, 60, 20, "Top");
        topCam->type(FL_RADIO_BUTTON);
        topCam->value(0);
        topCam->selection_color((Fl_Color)3);
		topCam->callback((Fl_Callback*)damageCB,this);
		camGroup->end();

		pty += 30;

		// browser to select spline types
		splineBrowser = new Fl_Browser(605,pty,120,75,"Spline Type");
		splineBrowser->type(2);		// select
		splineBrowser->callback((Fl_Callback*)damageCB,this);
		splineBrowser->add("Linear");
		splineBrowser->add("Cardinal Cubic");
		splineBrowser->select(2);

		pty += 110;

		// add and delete points
		Fl_Button* ap = new Fl_Button(605,pty,80,20,"Add Point");
		ap->callback((Fl_Callback*)addPointCB,this);
		Fl_Button* dp = new Fl_Button(690,pty,80,20,"Delete Point");
		dp->callback((Fl_Callback*)deletePointCB,this);

		pty += 25;
		// reset the points
		resetButton = new Fl_Button(735,pty,60,20,"Reset");
		resetButton->callback((Fl_Callback*)resetCB,this);
		Fl_Button* loadb = new Fl_Button(605,pty,60,20,"Load");
		loadb->callback((Fl_Callback*) loadCB, this);
		Fl_Button* saveb = new Fl_Button(670,pty,60,20,"Save");
		saveb->callback((Fl_Callback*) saveCB, this);

		pty += 25;
		// roll the points
		Fl_Button* rx = new Fl_Button(605,pty,30,20,"R+X");
		rx->callback((Fl_Callback*)rpxCB,this);
		Fl_Button* rxp = new Fl_Button(635,pty,30,20,"R-X");
		rxp->callback((Fl_Callback*)rmxCB,this);
		Fl_Button* rz = new Fl_Button(670,pty,30,20,"R+Z");
		rz->callback((Fl_Callback*)rpzCB,this);
		Fl_Button* rzp = new Fl_Button(700,pty,30,20,"R-Z");
		rzp->callback((Fl_Callback*)rmzCB,this);

		pty+= 40;

		tension = new Fl_Value_Slider(655, pty, 140, 20, "tension");
		tension->range(0,1);
		tension->value(0.5);
		tension->align(FL_ALIGN_LEFT);
		tension->type(FL_HORIZONTAL);
		tension->callback((Fl_Callback*)damageCB, this);

		pty += 30;

		//add number of cards
		carNum = new Fl_Counter(605, pty, 75, 20, "number of cars");
		carNum->type(FL_SIMPLE_COUNTER);
		carNum->step(1);
		carNum->minimum(0);
		carNum->callback((Fl_Callback*)damageCB, this);

		//regenerate randowm background
		resetEnvButtom = new Fl_Button(685, pty, 100, 20, "reset ENV");
		resetEnvButtom->callback((Fl_Callback*)resetEnv, this);

		//change the type of rail
		pty += 50;
		trackBrowser = new Fl_Browser(605, pty, 120, 55, "Rail Type");
		trackBrowser->type(2); // select
		trackBrowser->callback((Fl_Callback*)damageCB, this);
		trackBrowser->add("Simple");
		trackBrowser->add("Dual Rail");
		trackBrowser->select(2);
		//Ties
		ties = new Fl_Button(730, pty, 40, 20, "Ties");
		togglify(ties, 1);

			
		pty += 70;
		//the real speed
		speedDisplay = new Fl_Output(680, pty, 60, 20, "Real Speed:");
		speedDisplay->align(FL_ALIGN_LEFT);
		speedDisplay->value("2");

		//whether physic is active
		physic = new Fl_Button(745, pty, 50, 20, "Physic");
		togglify(physic, 1);

		pty += 40;

		wave = new Fl_Button(605, pty, 50, 20, "Waves");
		togglify(wave, 1);
		waveHeight = new Fl_Value_Slider(655, pty, 80, 20, "WaveHeight");
		waveHeight->range(0, 10);
		waveHeight->value(5);
		waveHeight->type(FL_HORIZONTAL);
		waveHeight->callback((Fl_Callback*)damageCB, this);

		buildings = new Fl_Button(740, pty, 60, 20, "Buildings");
		togglify(buildings, 1);
		//how high is the wave


		// we need to make a little phantom widget to have things resize correctly
		Fl_Box* resizebox = new Fl_Box(600,595,200,5);
		widgets->resizable(resizebox);

		widgets->end();
	}
	end();	// done adding to this widget

	// set up callback on idle
	Fl::add_idle((void (*)(void*))runButtonCB,this);
}

// handy utility to make a button into a toggle
void TrainWindow::togglify(Fl_Button* b, int val)
{
    b->type(FL_TOGGLE_BUTTON);		// toggle
    b->value(val);		// turned off
    b->selection_color((Fl_Color)3); // yellow when pressed	
	b->callback((Fl_Callback*)damageCB,this);
}

void TrainWindow::damageMe()
{
	if (trainView->selectedCube >= ((int)world.points.size()))
		trainView->selectedCube = 0;
	trainView->damage(1);
}

/////////////////////////////////////////////////////
// this will get called (approximately) 30 times per second
// if the run button is pressed
void TrainWindow::advanceTrain(float dir)
{
	//move the environment forward
	trainView->enValue = (trainView->enValue + 1) % 200;

	vector<float> list = trainView->distanceList;
	//get the total length
	float total = 0;
	for (int i = 0; i < list.size(); i++){
		total += list[i];
	}


	if (arcLength->value()){
		//get the arch Length table
		vector<float> arcLengthTable = trainView->arcLengthTable;

		//if there is physic, use the physic model
		if (physic->value() == 1){
			world.speed = PhysicModel::getVelocity((float)speed->value(),
				trainView->dirVector, trainView->trainPt);
			world.speed *= dir;
		}
		else{
			world.speed = dir * (float)speed->value();
		}


		//increase the travelled distance
		world.trainTravelled += world.speed;

		if (world.trainTravelled > total){
			world.trainTravelled -= total;
		}

		if (world.trainTravelled < 0){
			world.trainTravelled += total;
		}

		float last = 0;
		for (int i = 0; i < arcLengthTable.size(); i++){
			if (world.trainTravelled < arcLengthTable[i]){
				float diff = (world.trainTravelled - last) / (arcLengthTable[i] - last);
				int ptNum = 0;
				while (i >= 100){
					i -= 100;
					ptNum += 1;
				}
				world.trainU = (diff * 0.01) + (i * 0.01) + ptNum;
				world.trainPoint = ptNum;
				break;
			}
			last = arcLengthTable[i];
		}
	}
	else{
		world.speed = dir * (float)speed->value();
		world.trainU += dir * ((float)speed->value() * .05f);

		world.trainTravelled = (world.trainPoint + world.trainU)/list.size() * total;

		if (world.trainTravelled > total){
			world.trainTravelled -= total;
		}
	}

	//get the speed and display its value
	std::ostringstream ss;
	ss << world.speed;
	std::string s(ss.str());
	speedDisplay->value(s.c_str());
}