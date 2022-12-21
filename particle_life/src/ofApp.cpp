#include "ofApp.h"
#include "ofUtils.h"

#include <iostream>
#include <vector>
#include <random>
//#include "oneapi/tbb.h"


//Simulation parameters

// Create a group of particles with the same color and return it in a vector
colorGroup CreatePoints(const int num, ofColor color) noexcept
{
	colorGroup points;
	points.pos.reserve(num);
	points.vel.reserve(num);
	points.color = color;

	for (auto i = 0; i < num; i++)
	{
		int x = static_cast<int>(ofRandomWidth());
		int y = static_cast<int>(ofRandomHeight());	
		points.pos.emplace_back(x,y);
		points.vel.emplace_back(0.0F, 0.0F);
	}
		
	return points;
}

// compute and apply the interaction between two groups of particles
void ofApp::interaction(colorGroup& Group1, const colorGroup& Group2, 
		const float G, const float radius, bool boundsToggle) const noexcept
{
	
	assert(Group1.pos.size() % 64 == 0);
	assert(Group2.pos.size() % 64 == 0);
	
	const float g = G / -100;	// attraction coefficient

	//#pragma omp parallel
	for (size_t i = 0; i < Group1.pos.size(); i++)
	{
		float fx = 0.0F;	// force on x
		float fy = 0.0F;	// force on y
		
		
		for (size_t j = 0; j < Group2.pos.size(); j++)
		{
			if (Group1.pos[i] != Group2.pos[j])
			{
				const float distance_squared = ((Group1.pos[i].x - Group2.pos[j].x) * (Group1.pos[i].x - Group2.pos[j].x))
					+ ((Group1.pos[i].y - Group2.pos[j].y) * (Group1.pos[i].y - Group2.pos[j].y));

				// if distance < radius, apply the force, otherwise, force = 0
	//			const float force = distance_squared < radius*radius ? 1 / (std::max(std::numeric_limits<float>::epsilon(), std::sqrtf(distance_squared))) : 0.0F;
	//			fx += ((Group1.pos[i].x - Group2.pos[j].x) * force);
	//			fy += ((Group1.pos[i].y - Group2.pos[j].y) * force);
				const float force = distance_squared < radius* radius ? 1.0F / std::sqrtf(distance_squared) : 0.0F;
				fx += ((Group1.pos[i].x - Group2.pos[j].x) * force);
				fy += ((Group1.pos[i].y - Group2.pos[j].y) * force);
			}
		}

		// Wall Repel
		if (wallRepel > 0.0F)
		{
			Group1.vel[i].x += Group1.pos[i].x < wallRepel ? (wallRepel - Group1.pos[i].x) * 0.1 : 0.0F;	// x
			Group1.vel[i].y += Group1.pos[i].y < wallRepel ? (wallRepel - Group1.pos[i].y) * 0.1 : 0.0F;	// x
			Group1.vel[i].x += Group1.pos[i].x > boundWidth - wallRepel  ? (boundWidth - wallRepel - Group1.pos[i].x) * 0.1  : 0.0F; // y 
			Group1.vel[i].y += Group1.pos[i].y > boundHeight - wallRepel ? (boundHeight - wallRepel - Group1.pos[i].y) * 0.1 : 0.0F; // y			
		}

		// Viscosity & gravity
		Group1.vel[i].x = (Group1.vel[i].x + (fx * g)) * (1.0 - viscosity);
		Group1.vel[i].y = (Group1.vel[i].y + (fy * g)) * (1.0 - viscosity) + worldGravity;

		//Update position
		Group1.pos[i].x += Group1.vel[i].x;
		Group1.pos[i].y += Group1.vel[i].y;
	}

	// if "bounded" is checked then keep particles inside the window
	if (boundsToggle) {
		for (auto& p : Group1.pos)
		{
			p.x = std::min(std::max(p.x, 0.0F), static_cast<float>(boundWidth));
			p.y = std::min(std::max(p.y, 0.0F), static_cast<float>(boundHeight));
		}
	}
	
}
	

/**
 * @brief Generate new sets of points
 */
void ofApp::restart()
{
	// Ensure that the number of particles is a multiple of 64 in order to use the vectorized version of the interaction function
	numberSliderG = numberSliderG - (numberSliderG % 64);
	numberSliderR = numberSliderR - (numberSliderR % 64);
	numberSliderW = numberSliderW - (numberSliderW % 64);
	numberSliderY = numberSliderY - (numberSliderY % 64);
	
	assert(numberSliderG % 64 == 0);
	assert(numberSliderR % 64 == 0);
	assert(numberSliderW % 64 == 0);
	assert(numberSliderY % 64 == 0);
	
	// Create the groups of particles
	if (numberSliderG > 0) { green  = CreatePoints(numberSliderG, ofColor::green); }
	if (numberSliderR > 0) { red    = CreatePoints(numberSliderR,   ofColor::red);   }
	if (numberSliderW > 0) { white  = CreatePoints(numberSliderW, ofColor::white); }
	if (numberSliderY > 0) { yellow = CreatePoints(numberSliderY,  ofColor::yellow); }

	vbo.setVertexData(green.pos.data(),  green.pos.size(),  GL_STREAM_DRAW);
	vbo.setVertexData(red.pos.data(),    red.pos.size(),    GL_STREAM_DRAW);
	vbo.setVertexData(white.pos.data(),  white.pos.size(),  GL_STREAM_DRAW);
	vbo.setVertexData(yellow.pos.data(), yellow.pos.size(), GL_STREAM_DRAW);
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
	powerSliderGY = RandomFloat(-100, 100) * forceVariance;

	vSliderGG = RandomFloat(10, 200) * radiusVariance;
	vSliderGR = RandomFloat(10, 200) * radiusVariance;
	vSliderGW = RandomFloat(10, 200) * radiusVariance;
	vSliderGY = RandomFloat(10, 200) * radiusVariance;

	// RED
	//numberSliderR = RandomFloat(0, 3000);
	powerSliderRR = RandomFloat(-100, 100) * forceVariance;
	powerSliderRG = RandomFloat(-100, 100) * forceVariance;
	powerSliderRW = RandomFloat(-100, 100) * forceVariance;
	powerSliderRY = RandomFloat(-100, 100) * forceVariance;

	vSliderRG = RandomFloat(10, 200) * radiusVariance;
	vSliderRR = RandomFloat(10, 200) * radiusVariance;
	vSliderRW = RandomFloat(10, 200) * radiusVariance;
	vSliderRY = RandomFloat(10, 200) * radiusVariance;

	// WHITE
	// numberSliderW = RandomFloat(0, 3000);
	powerSliderWW = RandomFloat(-100, 100) * forceVariance;
	powerSliderWR = RandomFloat(-100, 100) * forceVariance;
	powerSliderWG = RandomFloat(-100, 100) * forceVariance;
	powerSliderWY = RandomFloat(-100, 100) * forceVariance;

	vSliderWG = RandomFloat(10, 200) * radiusVariance;
	vSliderWR = RandomFloat(10, 200) * radiusVariance;
	vSliderWW = RandomFloat(10, 200) * radiusVariance;
	vSliderWY = RandomFloat(10, 200) * radiusVariance;

	// yellow
	//numberSliderY = RandomFloat(0, 3000);
	powerSliderYY = RandomFloat(-100, 100) * forceVariance;
	powerSliderYW = RandomFloat(-100, 100) * forceVariance;
	powerSliderYR = RandomFloat(-100, 100) * forceVariance;
	powerSliderYG = RandomFloat(-100, 100) * forceVariance;

	vSliderYG = RandomFloat(10, 200) * radiusVariance;
	vSliderYR = RandomFloat(10, 200) * radiusVariance;
	vSliderYW = RandomFloat(10, 200) * radiusVariance;
	vSliderYY = RandomFloat(10, 200) * radiusVariance;
}

/// this is a cheap and quick way to save and load parameters (openFramework have betters ways but requires some additional library setups) 
// Dialog gui tested on windows machine only. Not sure if it works on Mac or Linux too.
void ofApp::saveSettings()
{
	const std::vector<float> settings = {
		powerSliderGG, powerSliderGR, powerSliderGW, powerSliderGY,
		vSliderGG, vSliderGR, vSliderGW, vSliderGY,
		powerSliderRG, powerSliderRR, powerSliderRW, powerSliderRY,
		vSliderRG, vSliderRR, vSliderRW, vSliderRY,
		powerSliderWG, powerSliderWR, powerSliderWW, powerSliderWY,
		vSliderWG, vSliderWR, vSliderWW, vSliderWY,
		powerSliderYG, powerSliderYR, powerSliderYW, powerSliderYY,
		vSliderYG, vSliderYR, vSliderYW, vSliderYY,
		static_cast<float>(numberSliderG),
		static_cast<float>(numberSliderR),
		static_cast<float>(numberSliderW),
		static_cast<float>(numberSliderY),
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
		powerSliderGY = p[3];
		vSliderGG = p[4];
		vSliderGR = p[5];
		vSliderGW = p[6];
		vSliderGY = p[7];
		powerSliderRG = p[8];
		powerSliderRR = p[9];
		powerSliderRW = p[10];
		powerSliderRY = p[11];
		vSliderRG = p[12];
		vSliderRR = p[13];
		vSliderRW = p[14];
		vSliderRY = p[15];
		powerSliderWG = p[16];
		powerSliderWR = p[17];
		powerSliderWW = p[18];
		powerSliderWY = p[19];
		vSliderWG = p[20];
		vSliderWR = p[21];
		vSliderWW = p[22];
		vSliderWY = p[23];
		powerSliderYG = p[24];
		powerSliderYR = p[25];
		powerSliderYW = p[26];
		powerSliderYY = p[27];
		vSliderYG = p[28];
		vSliderYR = p[29];
		vSliderYW = p[30];
		vSliderYY = p[31];
		numberSliderG = static_cast<int>(p[32]);
		numberSliderR = static_cast<int>(p[33]);
		numberSliderW = static_cast<int>(p[34]);
		numberSliderY = static_cast<int>(p[35]);
		viscoSlider = p[36];
	}
	probabilitySlider = 100;
	restart();
}


//------------------------------GUI initialization------------------------------
void ofApp::setup()
{
	
	lastTime = clock();
	lastTime_draw = lastTime;
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
	qtyGroup.add(numberSliderY.setup("Yellow", pnumberSliderY, 0, 10000));
	gui.add(&qtyGroup);

	// GREEN
	greenGroup.setup("Green");
	greenGroup.add(powerSliderGG.setup("green x green:", ppowerSliderGG, -100, 100));
	greenGroup.add(powerSliderGR.setup("green x red:", ppowerSliderGR, -100, 100));
	greenGroup.add(powerSliderGW.setup("green x white:", ppowerSliderGW, -100, 100));
	greenGroup.add(powerSliderGY.setup("green x yellow:", ppowerSliderGY, -100, 100));

	greenGroup.add(vSliderGG.setup("radius g x g:", pvSliderGG, 10, 500));
	greenGroup.add(vSliderGR.setup("radius g x r:", pvSliderGR, 10, 500));
	greenGroup.add(vSliderGW.setup("radius g x w:", pvSliderGW, 10, 500));
	greenGroup.add(vSliderGY.setup("radius g x y:", pvSliderGY, 10, 500));

	greenGroup.minimize();
	gui.add(&greenGroup);

	// RED
	redGroup.setup("Red");
	redGroup.add(powerSliderRR.setup("red x red:", ppowerSliderRR, -100, 100));
	redGroup.add(powerSliderRG.setup("red x green:", ppowerSliderRG, -100, 100));
	redGroup.add(powerSliderRW.setup("red x white:", ppowerSliderRW, -100, 100));
	redGroup.add(powerSliderRY.setup("red x yellow:", ppowerSliderRY, -100, 100));

	redGroup.add(vSliderRG.setup("radius r x g:", pvSliderRG, 10, 500));
	redGroup.add(vSliderRR.setup("radius r x r:", pvSliderRR, 10, 500));
	redGroup.add(vSliderRW.setup("radius r x w:", pvSliderRW, 10, 500));
	redGroup.add(vSliderRY.setup("radius r x y:", pvSliderRY, 10, 500));

	redGroup.minimize();
	gui.add(&redGroup);

	// WHITE
	whiteGroup.setup("White");
	whiteGroup.add(powerSliderWW.setup("white x white:", ppowerSliderWW, -100, 100));
	whiteGroup.add(powerSliderWR.setup("white x red:", ppowerSliderWR, -100, 100));
	whiteGroup.add(powerSliderWG.setup("white x green:", ppowerSliderWG, -100, 100));
	whiteGroup.add(powerSliderWY.setup("white x yellow:", ppowerSliderWY, -100, 100));

	whiteGroup.add(vSliderWG.setup("radius w x g:", pvSliderWG, 10, 500));
	whiteGroup.add(vSliderWR.setup("radius w x r:", pvSliderWR, 10, 500));
	whiteGroup.add(vSliderWW.setup("radius w x w:", pvSliderWW, 10, 500));
	whiteGroup.add(vSliderWY.setup("radius w x y:", pvSliderWY, 10, 500));

	whiteGroup.minimize();
	gui.add(&whiteGroup);

	// yellow
	yellowGroup.setup("Yellow");
	yellowGroup.add(powerSliderYY.setup("yellow x yellow:", ppowerSliderYY, -100, 100));
	yellowGroup.add(powerSliderYW.setup("yellow x white:", ppowerSliderYW, -100, 100));
	yellowGroup.add(powerSliderYR.setup("yellow x red:", ppowerSliderYR, -100, 100));
	yellowGroup.add(powerSliderYG.setup("yellow x green:", ppowerSliderYG, -100, 100));

	yellowGroup.add(vSliderYG.setup("radius y x g:", pvSliderYG, 10, 500));
	yellowGroup.add(vSliderYR.setup("radius y x r:", pvSliderYR, 10, 500));
	yellowGroup.add(vSliderYW.setup("radius y x w:", pvSliderYW, 10, 500));
	yellowGroup.add(vSliderYY.setup("radius y x y:", pvSliderYY, 10, 500));

	yellowGroup.minimize();
	gui.add(&yellowGroup);

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
	probability  = probabilitySlider;
	viscosity    = viscoSlider;
	worldGravity = gravitySlider;
	wallRepel    = wallRepelSlider;
	evoChance    = evoProbSlider;
	evoAmount    = evoAmountSlider;
	
	physic_begin = clock();
	boundHeight  = ofGetHeight();
	boundWidth   = ofGetWidth();


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

	/*
	oneapi::tbb::parallel_invoke(
		[&] { interaction(red,   red,   powerSliderRR, vSliderRR, boundsToggle); },
		[&] { interaction(red,   green, powerSliderRR, vSliderRG, boundsToggle); },
		[&] { interaction(red,   yellow,  powerSliderRR, vSliderRY, boundsToggle); },
		[&] { interaction(red,   white, powerSliderRR, vSliderRW, boundsToggle); },
		[&] { interaction(green, red,   powerSliderGR, vSliderGR, boundsToggle); },
		[&] { interaction(green, green, powerSliderGG, vSliderGG, boundsToggle); },
		[&] { interaction(green, yellow,  powerSliderGY, vSliderGY, boundsToggle); },
		[&] { interaction(green, white, powerSliderGW, vSliderGW, boundsToggle); },
		[&] { interaction(yellow,  red,   powerSliderYR, vSliderYR, boundsToggle); },
		[&] { interaction(yellow,  green, powerSliderYG, vSliderYG, boundsToggle); },
		[&] { interaction(yellow,  yellow,  powerSliderYY, vSliderYY, boundsToggle); },
		[&] { interaction(yellow,  white, powerSliderYW, vSliderYW, boundsToggle); },
		[&] { interaction(white, red,   powerSliderWR, vSliderWR, boundsToggle); },
		[&] { interaction(white, green, powerSliderWG, vSliderWG, boundsToggle); },
		[&] { interaction(white, yellow,  powerSliderWY, vSliderWY, boundsToggle); },
		[&] { interaction(white, white, powerSliderWW, vSliderWW, boundsToggle); }
	);
	*/

		interaction(red,   red,   powerSliderRR, vSliderRR, boundsToggle); 
		interaction(red,   green, powerSliderRR, vSliderRG, boundsToggle);
		interaction(red,   yellow,  powerSliderRR, vSliderRY, boundsToggle); 
		interaction(red,   white, powerSliderRR, vSliderRW, boundsToggle);
		interaction(green, red,   powerSliderGR, vSliderGR, boundsToggle);
		interaction(green, green, powerSliderGG, vSliderGG, boundsToggle);
		interaction(green, yellow,  powerSliderGY, vSliderGY, boundsToggle);
		interaction(green, white, powerSliderGW, vSliderGW, boundsToggle);
		interaction(yellow,  red,   powerSliderYR, vSliderYR, boundsToggle);
		interaction(yellow,  green, powerSliderYG, vSliderYG, boundsToggle);
		interaction(yellow,  yellow,  powerSliderYY, vSliderYY, boundsToggle);
		interaction(yellow,  white, powerSliderYW, vSliderYW, boundsToggle);
		interaction(white, red,   powerSliderWR, vSliderWR, boundsToggle);
		interaction(white, green, powerSliderWG, vSliderWG, boundsToggle);
		interaction(white, yellow,  powerSliderWY, vSliderWY, boundsToggle);
		interaction(white, white, powerSliderWW, vSliderWW, boundsToggle);
		
	
	if (save) { saveSettings(); }
	if (load) { loadSettings(); }
	physic_delta = clock() - physic_begin;
}

//--------------------------------------------------------------
void ofApp::draw()
{
	//fps counter
	cntFps++;
	now = clock();
	delta = now - lastTime;
	delta_draw = now - lastTime_draw;

		if (motionBlurToggle)
		{
			ofSetColor(0, 0, 0, 64);
			ofDrawRectangle(0, 0, boundWidth, boundHeight);
		}
		else
		{
			ofClear(0);
		}

		//Time step
		if (delta >= 1000)
		{
			lastTime = now;
			fps.setup("FPS", to_string(static_cast<int>((1000 / static_cast<float>(delta)) * cntFps)));
			physicLabel.setup("physics (ms)", to_string(physic_delta));

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
			if (numberSliderY > 0) { Draw(yellow); }
			lastTime_draw = now;

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
