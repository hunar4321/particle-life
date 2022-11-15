#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#define GRID_DIV 4

/*
 * for collision detection :
 * if (distance(x center, x line) < radius) then intersect 
 */

struct point
{
	point(float _x, float _y, const int _r, const int _g, const int _b) : x(_x), y(_y), r(_r), g(_g), b(_b) {}

	//Position
	float x;
	float y;

	//Velocity
	float vx = 0;
	float vy = 0;

	//Color
	const int r;
	const int g;
	const int b;

	int gridId = -1;

	void draw() const
	{
		ofSetColor(r, g, b, 100); //set particle color + some alpha
		ofDrawCircle(x, y, 2.0F); //draw a point at x,y coordinates, the size of a 2 pixels
	}
};

struct grid
{
	const int gridSize = GRID_DIV * GRID_DIV; // must be a power of 2
};

//---------------------------------------------CONFIGURE GUI---------------------------------------------//
class ofApp final : public ofBaseApp
{
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;
	void restart();
	void random();
	void saveSettings();
	void loadSettings();
	void interaction(std::vector<point>* Group1, const std::vector<point>* Group2, float G, float radius);

	ofxPanel gui;

	ofxGuiGroup globalGroup;
	ofxGuiGroup qtyGroup;
	ofxGuiGroup redGroup;
	ofxGuiGroup greenGroup;
	ofxGuiGroup blueGroup;
	ofxGuiGroup whiteGroup;

	ofxButton resetButton;
	ofxButton selectButton;
	ofxButton randomChoice;
	ofxButton save;
	ofxButton load;

	ofxToggle boundsToggle;
	ofxToggle modelToggle;
	ofxToggle motionBlurToggle;

	// some experimental stuff here
	ofxGuiGroup expGroup;
	ofxToggle evoToggle;
	ofxFloatSlider evoProbSlider;
	ofxFloatSlider evoAmountSlider;
	float evoChance = 1;
	float evoAmount = 1;
	ofxToggle radiusToogle;
	ofxLabel physicLabel;
	//end of experimental

	ofxIntSlider probabilitySlider;
	unsigned int probability = 100;

	ofxIntSlider numberSliderR;
	ofxIntSlider numberSliderG;
	ofxIntSlider numberSliderW;
	ofxIntSlider numberSliderB;

	int pnumberSliderR = 1000;
	int pnumberSliderG = 1000;
	int pnumberSliderW = 1000;
	int pnumberSliderB = 1000;

	ofxFloatSlider viscoSlider;
	ofxFloatSlider gravitySlider;
	ofxFloatSlider wallRepelSlider;

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
	ofxLabel fps;

	// simulation bounds
	int boundWidth = 1600;
	int boundHeight = 900;

	float viscosity = 0.5F;
	float worldGravity = 0.0F;
	float forceVariance = 0.8F;
	float radiusVariance = 0.6F;
	float wallRepel = 20.0F;
};
