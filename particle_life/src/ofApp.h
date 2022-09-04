#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include <fstream>

struct Point {
    //Position
	float x = 0;
	float y = 0;

	//Velocity
	float vx = 0;
	float vy = 0;

	//Color
	int r = 0;
	int g = 0;
	int b = 0;

	Point(int _x, int _y, int _r, int _g, int _b) {
		x = _x;
		y = _y;
		r = _r;
		g = _g;
		b = _b;
	}

	void Draw() {
		ofSetColor(r, g, b);  //set particle color
		ofDrawCircle((int)x, (int)y, 3); //draw a point at x,y coordinates, the size of a 3 pixels
	}
};


//---------------------------------------------CONFIGURE GUI---------------------------------------------//
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void restart();
		void random();
		void saveSettings();
		void loadSettings();
		void Interaction(std::vector<Point>* Group1, std::vector<Point>* Group2, float m, float radius);

		ofxPanel gui;

		ofxButton resetButton;
		ofxButton selectButton;
		ofxButton randomChoice;
		ofxButton save;
		ofxButton load;
		ofxToggle boundsToggle;
		ofxToggle modelToggle;

		ofxIntSlider numberSliderR;
		ofxIntSlider numberSliderG;
		ofxIntSlider numberSliderW;
		ofxIntSlider numberSliderB;

		int pnumberSliderR = 1000;
		int pnumberSliderG = 1000;
		int pnumberSliderW = 1000;
		int pnumberSliderB = 1000;

		ofxFloatSlider viscoSlider;

		ofxFloatSlider powerSliderRR;
		ofxFloatSlider powerSliderRG;
		ofxFloatSlider powerSliderRW;
		ofxFloatSlider powerSliderRB;

		float ppowerSliderRR = 0;
		float ppowerSliderRG = 0;
		float ppowerSliderRW = 0;
		float ppowerSliderRB = 0;

		ofxFloatSlider powerSliderGR;
		ofxFloatSlider powerSliderGG;
		ofxFloatSlider powerSliderGW;
		ofxFloatSlider powerSliderGB;

		float ppowerSliderGR = 0;
		float ppowerSliderGG = 0;
		float ppowerSliderGW = 0;
		float ppowerSliderGB = 0;


		ofxFloatSlider powerSliderWR;
		ofxFloatSlider powerSliderWG;
		ofxFloatSlider powerSliderWW;
		ofxFloatSlider powerSliderWB;

		float ppowerSliderWR = 0;
		float ppowerSliderWG = 0;
		float ppowerSliderWW = 0;
		float ppowerSliderWB = 0;

		ofxFloatSlider powerSliderBR;
		ofxFloatSlider powerSliderBG;
		ofxFloatSlider powerSliderBW;
		ofxFloatSlider powerSliderBB;

		float ppowerSliderBR = 0;
		float ppowerSliderBG = 0;
		float ppowerSliderBW = 0;
		float ppowerSliderBB = 0;

		ofxFloatSlider vSliderRR;
		ofxFloatSlider vSliderRG;
		ofxFloatSlider vSliderRW;
		ofxFloatSlider vSliderRB;

		float pvSliderRR = 180;
		float pvSliderRG = 180;
		float pvSliderRW = 180;
		float pvSliderRB = 180;

		ofxFloatSlider vSliderGR;
		ofxFloatSlider vSliderGG;
		ofxFloatSlider vSliderGW;
		ofxFloatSlider vSliderGB;

		float pvSliderGR = 180;
		float pvSliderGG = 180;
		float pvSliderGW = 180;
		float pvSliderGB = 180;

		ofxFloatSlider vSliderWR;
		ofxFloatSlider vSliderWG;
		ofxFloatSlider vSliderWW;
		ofxFloatSlider vSliderWB;

		float pvSliderWR = 180;
		float pvSliderWG = 180;
		float pvSliderWW = 180;
		float pvSliderWB = 180;

		ofxFloatSlider vSliderBR;
		ofxFloatSlider vSliderBG;
		ofxFloatSlider vSliderBW;
		ofxFloatSlider vSliderBB;

		float pvSliderBR = 180;
		float pvSliderBG = 180;
		float pvSliderBW = 180;
		float pvSliderBB = 180;
		ofxLabel labelG;
		ofxLabel labelR;
		ofxLabel labelW;
		ofxLabel labelB;

		ofxLabel aboutL1;
		ofxLabel aboutL2;
		ofxLabel aboutL3;

		// simulation bounds
		float boundWidth = 1600;
		float boundHeight = 900;
	
		float viscosity = 0.5;
		float forceVariance = 0.8;
		float radiusVariance = 0.6;
	
		ofxLabel fps;
};
