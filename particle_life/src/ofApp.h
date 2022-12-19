#pragma once

#include "ofMain.h"
#include "ofxGui.h"
//#include <sdlt/sdlt.h>


// use a SOA that represents a group of particles with the same color
struct colorGroup {
	std::vector<ofVec2f> pos;
	std::vector<ofVec2f> vel;
	ofColor color;
};


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
	void interaction(colorGroup& Group1, const colorGroup& Group2, const float G, const float radius, bool boundsToggle) const noexcept;

	static float RandomFloat(const float a, const float b) { return a + (ofRandomuf() * (b - a)); }

	void Draw(colorGroup group)
	{
		ofSetColor(group.color);
		vbo.updateVertexData(group.pos.data(), group.pos.size());
		vbo.draw(GL_POINTS, 0, group.pos.size());

	}
	
	ofxPanel gui;
	ofVbo vbo;

	colorGroup green;
	colorGroup red;
	colorGroup white;
	colorGroup yellow;

	int cntFps = 0;
	clock_t now, lastTime, delta;
	clock_t lastTime_draw, delta_draw;
	clock_t physic_begin, physic_delta;

#pragma region guigroup
	ofxGuiGroup globalGroup;
	ofxGuiGroup qtyGroup;
	ofxGuiGroup redGroup;
	ofxGuiGroup greenGroup;
	ofxGuiGroup yellowGroup;
	ofxGuiGroup whiteGroup;
#pragma endregion guigroup

	ofxButton resetButton;
	ofxButton selectButton;
	ofxButton randomChoice;
	ofxButton save;
	ofxButton load;

	ofxToggle boundsToggle;
	ofxToggle modelToggle;

#pragma region some experimental features
	ofxGuiGroup expGroup;
	ofxToggle evoToggle;
	ofxFloatSlider evoProbSlider;
	ofxFloatSlider evoAmountSlider;
	float evoChance = 1;
	float evoAmount = 1;
	ofxToggle radiusToogle;
	ofxLabel physicLabel;
	ofxIntSlider probabilitySlider;
	unsigned int probability = 100;
	ofxToggle motionBlurToggle;
#pragma endregion some experimental features



#pragma region slider
	ofxIntSlider numberSliderR;
	ofxIntSlider numberSliderG;
	ofxIntSlider numberSliderW;
	ofxIntSlider numberSliderY;

	ofxFloatSlider viscoSlider;
	ofxFloatSlider gravitySlider;
	ofxFloatSlider wallRepelSlider;

	ofxFloatSlider powerSliderRR;
	ofxFloatSlider powerSliderRG;
	ofxFloatSlider powerSliderRW;
	ofxFloatSlider powerSliderRY;
	
	ofxFloatSlider powerSliderGR;
	ofxFloatSlider powerSliderGG;
	ofxFloatSlider powerSliderGW;
	ofxFloatSlider powerSliderGY;
	
	ofxFloatSlider powerSliderWR;
	ofxFloatSlider powerSliderWG;
	ofxFloatSlider powerSliderWW;
	ofxFloatSlider powerSliderWY;
	
	ofxFloatSlider powerSliderYR;
	ofxFloatSlider powerSliderYG;
	ofxFloatSlider powerSliderYW;
	ofxFloatSlider powerSliderYY;
	
	ofxFloatSlider vSliderRR;
	ofxFloatSlider vSliderRG;
	ofxFloatSlider vSliderRW;
	ofxFloatSlider vSliderRY;
	
	ofxFloatSlider vSliderGR;
	ofxFloatSlider vSliderGG;
	ofxFloatSlider vSliderGW;
	ofxFloatSlider vSliderGY;
	
	ofxFloatSlider vSliderWR;
	ofxFloatSlider vSliderWG;
	ofxFloatSlider vSliderWW;
	ofxFloatSlider vSliderWY;

	ofxFloatSlider vSliderYR;
	ofxFloatSlider vSliderYG;
	ofxFloatSlider vSliderYW;
	ofxFloatSlider vSliderYY;

	vector<ofxFloatSlider*> powersliders = {
		&powerSliderRR, &powerSliderRG, &powerSliderRY, &powerSliderRW,
		&powerSliderGR, &powerSliderGG, &powerSliderGY, &powerSliderGW,
		&powerSliderYR, &powerSliderYG, &powerSliderYY, &powerSliderYW,
		&powerSliderWR, &powerSliderWG, &powerSliderWY, &powerSliderWW,
	};

	vector<ofxFloatSlider*> vsliders = {
		&vSliderRR, &vSliderRG, &vSliderRY, &vSliderRW,
		&vSliderGR, &vSliderGG, &vSliderGY, &vSliderGW,
		&vSliderYR, &vSliderYG, &vSliderYY, &vSliderYW,
		&vSliderWR, &vSliderWG, &vSliderWY, &vSliderWW,
	};

#pragma endregion slider

#pragma region slider values
	unsigned int pnumberSliderR = 1000;
	unsigned int pnumberSliderG = 1000;
	unsigned int pnumberSliderW = 1000;
	unsigned int pnumberSliderY = 1000;

	float ppowerSliderRR = 0;
	float ppowerSliderRG = 0;
	float ppowerSliderRW = 0;
	float ppowerSliderRY = 0;
	
	float ppowerSliderGR = 0;
	float ppowerSliderGG = 0;
	float ppowerSliderGW = 0;
	float ppowerSliderGY = 0;

	float ppowerSliderWR = 0;
	float ppowerSliderWG = 0;
	float ppowerSliderWW = 0;
	float ppowerSliderWY = 0;

	float ppowerSliderYR = 0;
	float ppowerSliderYG = 0;
	float ppowerSliderYW = 0;
	float ppowerSliderYY = 0;

	float pvSliderRR = 180;
	float pvSliderRG = 180;
	float pvSliderRW = 180;
	float pvSliderRY = 180;

	float pvSliderGR = 180;
	float pvSliderGG = 180;
	float pvSliderGW = 180;
	float pvSliderGY = 180;

	float pvSliderWR = 180;
	float pvSliderWG = 180;
	float pvSliderWW = 180;
	float pvSliderWY = 180;

	float pvSliderYR = 180;
	float pvSliderYG = 180;
	float pvSliderYW = 180;
	float pvSliderYY = 180;

#pragma endregion slider values

	ofxLabel labelG;
	ofxLabel labelR;
	ofxLabel labelW;
	ofxLabel labelY;
	
	ofxLabel aboutL1;
	ofxLabel aboutL2;
	ofxLabel aboutL3;
	ofxLabel fps;

	std::string fps_text;
	std::string physic_text;


	// simulation bounds
	unsigned int boundWidth = 1600;
	unsigned int boundHeight = 900;

	float viscosity = 0.5F;
	float worldGravity = 0.0F;
	float forceVariance = 0.8F;
	float radiusVariance = 0.6F;
	float wallRepel = 10.0F;
};
