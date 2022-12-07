#include "ofApp.h"
#include "ofUtils.h"

#include <iostream>
#include <vector>
#include <random>
#include "oneapi/tbb.h"


//int countThresh = 0;
std::string fps_text;
std::string physic_text;

//Simulation parameters
int cntFps = 0;
clock_t now, lastTime, delta;
clock_t physic_begin, physic_delta;

//Particle groups by color
colorGroup green;
colorGroup red;
colorGroup white;
colorGroup blue;


colorGroup CreatePoints(const int num, ofColor color)
{
	colorGroup points;
	points.pos.reserve(num);
	points.vx.reserve(num);
	points.vy.reserve(num);
	points.color = color;

	for (auto i = 0; i < num; i++)
	{
		int x = static_cast<int>(ofRandomWidth());
		int y = static_cast<int>(ofRandomHeight());
		points.pos.emplace_back(x,y);
		points.vx.emplace_back(0.0F);
		points.vy.emplace_back(0.0F);
	}
	return points;
}

void ofApp::interaction(colorGroup& Group1, const colorGroup& Group2, 
		const float G, const float radius, bool boundsToggle) const
{
	
	assert(Group1.pos.size() % 64 == 0);
	assert(Group2.pos.size() % 64 == 0);
	
	const float g = G / -100;	// attraction coefficient

//		oneapi::tbb::parallel_for(
//			oneapi::tbb::blocked_range<size_t>(0, group1size), 
//			[&Group1, &Group2, group1size, group2size, radius, g, this]
//			(const oneapi::tbb::blocked_range<size_t>& r) {

	for (size_t i = 0; i < Group1.pos.size(); i++)
	{
		float fx = 0;	// force on x
		float fy = 0;	// force on y
		
		for (size_t j = 0; j < Group2.pos.size(); j++)
		{
			const float distance = Group1.pos[i].distance(Group2.pos[j]);
			if ((distance < radius)) {
				const float force = 1 / std::max(std::numeric_limits<float>::epsilon(), distance);	// avoid dividing by zero
				fx += ((Group1.pos[i].x - Group2.pos[j].x) * force);
				fy += ((Group1.pos[i].y - Group2.pos[j].y) * force);
			}
		}

		// Wall Repel
		if (wallRepel > 0.0F)
		{
			if (Group1.pos[i].x < wallRepel) Group1.vx[i] += (wallRepel - Group1.pos[i].x) * 0.1;
			if (Group1.pos[i].x > boundWidth - wallRepel) Group1.vx[i] += (boundWidth - wallRepel - Group1.pos[i].x) * 0.1;
			if (Group1.pos[i].y < wallRepel) Group1.vy[i] += (wallRepel - Group1.pos[i].y) * 0.1;
			if (Group1.pos[i].y > boundHeight - wallRepel) Group1.vy[i] += (boundHeight - wallRepel - Group1.pos[i].y) * 0.1;
		}

		// Viscosity & gravity
		Group1.vx[i] = (Group1.vx[i] + (fx * g)) * (1.0 - viscosity);
		Group1.vy[i] = (Group1.vy[i] + (fy * g)) * (1.0 - viscosity) + worldGravity;
//		Group1.vx[i] = std::fmaf(Group1.vx[i], (1.0F - viscosity), std::fmaf(fx, g, 0.0F));
//		Group1.vy[i] = std::fmaf(Group1.vy[i], (1.0F - viscosity), std::fmaf(fy, g, worldGravity));

		//Update position
		Group1.pos[i].x += Group1.vx[i];
		Group1.pos[i].y += Group1.vy[i];
	}

	if (boundsToggle) {
		for (auto& p : Group1.pos)
		{
			p.x = std::min(std::max(p.x, 0.0F), static_cast<float>(boundWidth));
			p.y = std::min(std::max(p.y, 0.0F), static_cast<float>(boundHeight));
		}
	}
	
}
	

/* omp end parallel */

/**
 * @brief Generate new sets of points
 */
void ofApp::restart()
{
	numberSliderG = numberSliderG - (numberSliderG % 64);
	numberSliderR = numberSliderR - (numberSliderR % 64);
	numberSliderW = numberSliderW - (numberSliderW % 64);
	numberSliderB = numberSliderB - (numberSliderB % 64);
	assert(numberSliderG % 64 == 0);
	assert(numberSliderR % 64 == 0);
	assert(numberSliderW % 64 == 0);
	assert(numberSliderB % 64 == 0);
	
	if (numberSliderG > 0) { green = CreatePoints(numberSliderG, ofColor(100, 250, 10, 255)); }
	if (numberSliderR > 0) { red = CreatePoints(numberSliderR, ofColor(250, 10, 100, 255)); }
	if (numberSliderW > 0) { white = CreatePoints(numberSliderW, ofColor(250, 250, 250, 255)); }
	if (numberSliderB > 0) { blue = CreatePoints(numberSliderB, ofColor(100, 100, 250, 255)); }
}


/**
 * @brief Generate initial simulation parameters
 */
void ofApp::random()
{
	// GREEN
	//numberSliderG = RandomFloat(0, 3000);
	powerSliderGG = RandomFloat(-100, 100) * forceVariance;
	powerSliderGR = RandomFloat(-100, 100) * forceVariance;
	powerSliderGW = RandomFloat(-100, 100) * forceVariance;
	powerSliderGB = RandomFloat(-100, 100) * forceVariance;

	vSliderGG = RandomFloat(10, 200) * radiusVariance;
	vSliderGR = RandomFloat(10, 200) * radiusVariance;
	vSliderGW = RandomFloat(10, 200) * radiusVariance;
	vSliderGB = RandomFloat(10, 200) * radiusVariance;

	// RED
	//numberSliderR = RandomFloat(0, 3000);
	powerSliderRR = RandomFloat(-100, 100) * forceVariance;
	powerSliderRG = RandomFloat(-100, 100) * forceVariance;
	powerSliderRW = RandomFloat(-100, 100) * forceVariance;
	powerSliderRB = RandomFloat(-100, 100) * forceVariance;

	vSliderRG = RandomFloat(10, 200) * radiusVariance;
	vSliderRR = RandomFloat(10, 200) * radiusVariance;
	vSliderRW = RandomFloat(10, 200) * radiusVariance;
	vSliderRB = RandomFloat(10, 200) * radiusVariance;

	// WHITE
	// numberSliderW = RandomFloat(0, 3000);
	powerSliderWW = RandomFloat(-100, 100) * forceVariance;
	powerSliderWR = RandomFloat(-100, 100) * forceVariance;
	powerSliderWG = RandomFloat(-100, 100) * forceVariance;
	powerSliderWB = RandomFloat(-100, 100) * forceVariance;

	vSliderWG = RandomFloat(10, 200) * radiusVariance;
	vSliderWR = RandomFloat(10, 200) * radiusVariance;
	vSliderWW = RandomFloat(10, 200) * radiusVariance;
	vSliderWB = RandomFloat(10, 200) * radiusVariance;

	// BLUE
	//numberSliderB = RandomFloat(0, 3000);
	powerSliderBB = RandomFloat(-100, 100) * forceVariance;
	powerSliderBW = RandomFloat(-100, 100) * forceVariance;
	powerSliderBR = RandomFloat(-100, 100) * forceVariance;
	powerSliderBG = RandomFloat(-100, 100) * forceVariance;

	vSliderBG = RandomFloat(10, 200) * radiusVariance;
	vSliderBR = RandomFloat(10, 200) * radiusVariance;
	vSliderBW = RandomFloat(10, 200) * radiusVariance;
	vSliderBB = RandomFloat(10, 200) * radiusVariance;
}

/// this is a cheap and quick way to save and load parameters (openFramework have betters ways but requires some additional library setups) 
// Dialog gui tested on windows machine only. Not sure if it works on Mac or Linux too.
void ofApp::saveSettings()
{
	const std::vector<float> settings = {
		powerSliderGG, powerSliderGR, powerSliderGW, powerSliderGB,
		vSliderGG, vSliderGR, vSliderGW, vSliderGB,
		powerSliderRG, powerSliderRR, powerSliderRW, powerSliderRB,
		vSliderRG, vSliderRR, vSliderRW, vSliderRB,
		powerSliderWG, powerSliderWR, powerSliderWW, powerSliderWB,
		vSliderWG, vSliderWR, vSliderWW, vSliderWB,
		powerSliderBG, powerSliderBR, powerSliderBW, powerSliderBB,
		vSliderBG, vSliderBR, vSliderBW, vSliderBB,
		static_cast<float>(numberSliderG),
		static_cast<float>(numberSliderR),
		static_cast<float>(numberSliderW),
		static_cast<float>(numberSliderB),
		viscoSlider
	};

	std::string save_path;
	ofFileDialogResult result = ofSystemSaveDialog("model.txt", "Save");
	if (result.bSuccess)
	{
		save_path = result.getPath();
	}
	else
	{
		ofSystemAlertDialog("Could not Save Model!");
	}
	ofstream myfile(save_path);
	if (myfile.is_open())
	{
		for (int i = 0; i < settings.size(); i++)
		{
			myfile << settings[i] << " ";
		}
		myfile.close();
		std::cout << "file saved successfully";
	}
	else
	{
		std::cout << "unable to save file!";
	}
}

// Dialog gui tested on windows machine only. Not sure if it works on Mac or Linux too.
void ofApp::loadSettings()
{
	std::string load_path;
	std::string text;
	ofFileDialogResult result = ofSystemLoadDialog("Load file", false, load_path);
	if (result.bSuccess)
	{
		load_path = result.getPath();
		std::ifstream t(load_path);
		std::stringstream loaded;
		loaded << t.rdbuf();
		text = loaded.str();
	}
	else
	{
		ofSystemAlertDialog("Could not Load the File!");
	}

	// split text by space and convert them to floats
	string word;
	std::vector<float> p;
	for (auto x : text)
	{
		if (x == ' ')
		{
			p.push_back(std::stof(word));
			word = "";
		}
		else
		{
			word += x;
		}
	}

	if (p.size() < 37)
	{
		// better checks needed
		ofSystemAlertDialog("Could not read the file!");
	}
	else
	{
		powerSliderGG = p[0];
		powerSliderGR = p[1];
		powerSliderGW = p[2];
		powerSliderGB = p[3];
		vSliderGG = p[4];
		vSliderGR = p[5];
		vSliderGW = p[6];
		vSliderGB = p[7];
		powerSliderRG = p[8];
		powerSliderRR = p[9];
		powerSliderRW = p[10];
		powerSliderRB = p[11];
		vSliderRG = p[12];
		vSliderRR = p[13];
		vSliderRW = p[14];
		vSliderRB = p[15];
		powerSliderWG = p[16];
		powerSliderWR = p[17];
		powerSliderWW = p[18];
		powerSliderWB = p[19];
		vSliderWG = p[20];
		vSliderWR = p[21];
		vSliderWW = p[22];
		vSliderWB = p[23];
		powerSliderBG = p[24];
		powerSliderBR = p[25];
		powerSliderBW = p[26];
		powerSliderBB = p[27];
		vSliderBG = p[28];
		vSliderBR = p[29];
		vSliderBW = p[30];
		vSliderBB = p[31];
		numberSliderG = static_cast<int>(p[32]);
		numberSliderR = static_cast<int>(p[33]);
		numberSliderW = static_cast<int>(p[34]);
		numberSliderB = static_cast<int>(p[35]);
		viscoSlider = p[36];
	}
	probabilitySlider = 100;
	restart();
}


//------------------------------GUI initialization------------------------------
void ofApp::setup()
{
	
	lastTime = clock();
	ofSetWindowTitle("Particle Life - www.brainxyz.com");
	ofSetVerticalSync(false);

	// Interface
	gui.setup("Settings");
	gui.loadFont("Arial", 12);
	gui.setWidthElements(300.0f);

	gui.add(fps.setup("FPS", "0"));

	gui.add(resetButton.setup("Restart"));
	gui.add(randomChoice.setup("Randomize (space bar)"));
	gui.add(save.setup("Save Model"));
	gui.add(load.setup("Load Model"));
	gui.add(modelToggle.setup("Show Model", false));

	globalGroup.setup("Global");
	globalGroup.add(viscoSlider.setup("Viscosity/Friction", viscosity, 0, 1));
	globalGroup.add(gravitySlider.setup("Gravity", worldGravity, -1, 1));
	globalGroup.add(wallRepelSlider.setup("Wall Repel", wallRepel, 0, 100));
	globalGroup.add(boundsToggle.setup("Bounded", false));

	gui.add(&globalGroup);

	// Quantity
	qtyGroup.setup("Quantity (require restart/randomize)");
	qtyGroup.add(numberSliderG.setup("Green", pnumberSliderG, 0, 10000));
	qtyGroup.add(numberSliderR.setup("Red", pnumberSliderR, 0, 10000));
	qtyGroup.add(numberSliderW.setup("White", pnumberSliderW, 0, 10000));
	qtyGroup.add(numberSliderB.setup("Blue", pnumberSliderB, 0, 10000));
	gui.add(&qtyGroup);

	// GREEN
	greenGroup.setup("Green");
	greenGroup.add(powerSliderGG.setup("green x green:", ppowerSliderGG, -100, 100));
	greenGroup.add(powerSliderGR.setup("green x red:", ppowerSliderGR, -100, 100));
	greenGroup.add(powerSliderGW.setup("green x white:", ppowerSliderGW, -100, 100));
	greenGroup.add(powerSliderGB.setup("green x blue:", ppowerSliderGB, -100, 100));

	greenGroup.add(vSliderGG.setup("radius g x g:", pvSliderGG, 10, 500));
	greenGroup.add(vSliderGR.setup("radius g x r:", pvSliderGR, 10, 500));
	greenGroup.add(vSliderGW.setup("radius g x w:", pvSliderGW, 10, 500));
	greenGroup.add(vSliderGB.setup("radius g x b:", pvSliderGB, 10, 500));

	greenGroup.minimize();
	gui.add(&greenGroup);

	// RED
	redGroup.setup("Red");
	redGroup.add(powerSliderRR.setup("red x red:", ppowerSliderRR, -100, 100));
	redGroup.add(powerSliderRG.setup("red x green:", ppowerSliderRG, -100, 100));
	redGroup.add(powerSliderRW.setup("red x white:", ppowerSliderRW, -100, 100));
	redGroup.add(powerSliderRB.setup("red x blue:", ppowerSliderRB, -100, 100));

	redGroup.add(vSliderRG.setup("radius r x g:", pvSliderRG, 10, 500));
	redGroup.add(vSliderRR.setup("radius r x r:", pvSliderRR, 10, 500));
	redGroup.add(vSliderRW.setup("radius r x w:", pvSliderRW, 10, 500));
	redGroup.add(vSliderRB.setup("radius r x b:", pvSliderRB, 10, 500));

	redGroup.minimize();
	gui.add(&redGroup);

	// WHITE
	whiteGroup.setup("White");
	whiteGroup.add(powerSliderWW.setup("white x white:", ppowerSliderWW, -100, 100));
	whiteGroup.add(powerSliderWR.setup("white x red:", ppowerSliderWR, -100, 100));
	whiteGroup.add(powerSliderWG.setup("white x green:", ppowerSliderWG, -100, 100));
	whiteGroup.add(powerSliderWB.setup("white x blue:", ppowerSliderWB, -100, 100));

	whiteGroup.add(vSliderWG.setup("radius w x g:", pvSliderWG, 10, 500));
	whiteGroup.add(vSliderWR.setup("radius w x r:", pvSliderWR, 10, 500));
	whiteGroup.add(vSliderWW.setup("radius w x w:", pvSliderWW, 10, 500));
	whiteGroup.add(vSliderWB.setup("radius w x b:", pvSliderWB, 10, 500));

	whiteGroup.minimize();
	gui.add(&whiteGroup);

	// BLUE
	blueGroup.setup("Blue");
	blueGroup.add(powerSliderBB.setup("blue x blue:", ppowerSliderBB, -100, 100));
	blueGroup.add(powerSliderBW.setup("blue x white:", ppowerSliderBW, -100, 100));
	blueGroup.add(powerSliderBR.setup("blue x red:", ppowerSliderBR, -100, 100));
	blueGroup.add(powerSliderBG.setup("blue x green:", ppowerSliderBG, -100, 100));

	blueGroup.add(vSliderBG.setup("radius b x g:", pvSliderBG, 10, 500));
	blueGroup.add(vSliderBR.setup("radius b x r:", pvSliderBR, 10, 500));
	blueGroup.add(vSliderBW.setup("radius b x w:", pvSliderBW, 10, 500));
	blueGroup.add(vSliderBB.setup("radius b x b:", pvSliderBB, 10, 500));

	blueGroup.minimize();
	gui.add(&blueGroup);

	expGroup.setup("Experimental");
	expGroup.add(evoToggle.setup("Evolve parameters", false));
	expGroup.add(evoProbSlider.setup("evo chance%", evoChance, 0, 100));
	expGroup.add(evoAmountSlider.setup("evo amount%%", evoAmount, 0, 100));
	//expGroup.add(radiusToogle.setup("infinite radius", false));
	//expGroup.add(probabilitySlider.setup("interaction prob%", probability, 1, 100));
	expGroup.add(motionBlurToggle.setup("Motion Blur", false));
	expGroup.add(physicLabel.setup("physic (ms)", "0"));

	expGroup.minimize();
	gui.add(&expGroup);

	ofSetBackgroundAuto(false);
	//ofDisableAlphaBlending();

	random();
	restart();
}

//------------------------------Update simulation with sliders values------------------------------
void ofApp::update()
{
	physic_begin = clock();
	probability = probabilitySlider;
	viscosity = viscoSlider;
	worldGravity = gravitySlider;
	wallRepel = wallRepelSlider;
	evoChance = evoProbSlider;
	evoAmount = evoAmountSlider;
	boundHeight = ofGetHeight();
	boundWidth = ofGetWidth();


	if (evoToggle && ofRandom(1.0F) < (evoChance / 100.0F))
	{
		for (auto& slider : powersliders) {
			*slider = *slider + ((ofRandom(2.0F) - 1.0F) * (slider->getMax() - slider->getMin()) * (evoAmount / 100.0F));
		}

		for (auto& slider : vsliders) {
			*slider = *slider + ((ofRandom(2.0F) - 1.0F) * (slider->getMax() - slider->getMin()) * (evoAmount / 100.0F));
			if (*slider < 0.0F) *slider = 0;
			if (*slider > 200.0F) *slider = 200.0F;
		}
	}
	oneapi::tbb::parallel_invoke(
		[&] { interaction(red,   red,   powerSliderRR, vSliderRR, boundsToggle); },
		[&] { interaction(red,   green, powerSliderRR, vSliderRG, boundsToggle); },
		[&] { interaction(red,   blue,  powerSliderRR, vSliderRB, boundsToggle); },
		[&] { interaction(red,   white, powerSliderRR, vSliderRW, boundsToggle); },
		[&] { interaction(green, red,   powerSliderGR, vSliderGR, boundsToggle); },
		[&] { interaction(green, green, powerSliderGG, vSliderGG, boundsToggle); },
		[&] { interaction(green, blue,  powerSliderGB, vSliderGB, boundsToggle); },
		[&] { interaction(green, white, powerSliderGW, vSliderGW, boundsToggle); },
		[&] { interaction(blue,  red,   powerSliderBR, vSliderBR, boundsToggle); },
		[&] { interaction(blue,  green, powerSliderBG, vSliderBG, boundsToggle); },
		[&] { interaction(blue,  blue,  powerSliderBB, vSliderBB, boundsToggle); },
		[&] { interaction(blue,  white, powerSliderBW, vSliderBW, boundsToggle); },
		[&] { interaction(white, red,   powerSliderWR, vSliderWR, boundsToggle); },
		[&] { interaction(white, green, powerSliderWG, vSliderWG, boundsToggle); },
		[&] { interaction(white, blue,  powerSliderWB, vSliderWB, boundsToggle); },
		[&] { interaction(white, white, powerSliderWW, vSliderWW, boundsToggle); }
	);

	if (save) { saveSettings(); }
	if (load) { loadSettings(); }
	physic_delta = clock() - physic_begin;
}

//--------------------------------------------------------------
void ofApp::draw()
{
	if (motionBlurToggle)
	{
		ofSetColor(0, 0, 0, 64);
		ofDrawRectangle(0, 0, boundWidth, boundHeight);
	}
	else
	{
		ofClear(0);
	}

	//fps counter
	cntFps++;
	now = clock();
	delta = now - lastTime;

	//Time step
	if (delta >= 1000)
	{
		lastTime = now;
		fps.setup("FPS", to_string(static_cast<int>((1000 / static_cast<float>(delta)) * cntFps)));
		physicLabel.setup("physics (ms)", to_string(physic_delta) );

		cntFps = 0;
	}

	//Check for GUI interaction
	if (resetButton) { restart(); }
	if (randomChoice)
	{
		random();
		restart();
	}
	if (numberSliderW > 0) { Draw(white); }
	if (numberSliderR > 0) { Draw(red); }
	if (numberSliderG > 0) { Draw(green); }
	if (numberSliderB > 0) { Draw(blue); }

	gui.draw();
}

void ofApp::keyPressed(int key)
{
	if(key == ' ')
	{
		random();
		restart();
	}
}
