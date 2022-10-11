#include "ofApp.h"

#include <iostream>
#include <vector>
#include <omp.h>

// parameters for image positions
float xshift = 400;
float yshift = 80;
float anchor = 0;
float length = 70;
float p1x = anchor+xshift;
float p1y = anchor+yshift;
float p2x = anchor+length+xshift;
float p2y = anchor+yshift;
float p3x = anchor+length+xshift;
float p3y = anchor+length+yshift;
float p4x = anchor + xshift;
float p4y = anchor + length + yshift;
float rr = 8;
int countThresh = 0;
std::string fps_text;

//Simulation parameters
int cntFps = 0;
clock_t now, lastTime, delta;

//Particle groups by color
std::vector<point> green;
std::vector<point> red;
std::vector<point> white;
std::vector<point> blue;


//Get random float in range <a,b>
float RandomFloat(float a, float b) {
	const float diff = b - a;
	const float r = ofRandomuf() * diff;
    return a + r;
}

//Draw all points from given group
void Draw(const std::vector<point>* points) {
    for (int i = 0; i < points->size(); i++) {
        (* points)[i].draw();
    }
}

//Generate a number of single colored points randomly distributed on canvas
std::vector<point> CreatePoints(int num, int r, int g, int b) {
    std::vector<point> points;
    for (int i = 0; i < num; i++) {
        int x = ofRandomWidth();
        int y = ofRandomHeight();
        points.emplace_back(x, y, r, g, b);
    }
    return points;
}

//Interaction between 2 particle groups
void ofApp::interaction(std::vector<point>* Group1, const std::vector<point>* Group2, float G, const float radius) {

    //Gravity coefficient
    const float g = G / -100;

    boundHeight = ofGetHeight();
    boundWidth = ofGetWidth();

    omp_set_num_threads(omp_get_num_procs());
    #pragma omp parallel for
    //Loop through first group of points
    for (int i = 0; i < Group1->size(); i++) {
        auto p1 = (*Group1)[i];
		//Force acting on particle
        float fx = 0;
        float fy = 0;
		//Loop through second group of points
        for (int j = 0; j < Group2->size(); j++) {
	        const auto p2 = (*Group2)[j];
			//Calculate the distance between points using Pythagorean theorem
	        const auto dx = p1.x - p2.x;
	        const auto dy = p1.y - p2.y;
	        const auto r = std::sqrt(dx * dx + dy * dy);
			
			//Calculate the force in given bounds. 
            if (r < radius && r > 0) {
                fx += (dx / r);
                fy += (dy / r);
            }
        }

		//Calculate new velocity
        p1.vx = (p1.vx + (fx * g)) * (1.0-viscosity);
        p1.vy = (p1.vy + (fy * g)) * (1.0-viscosity) + gravity;
		//Update position based on velocity
        p1.x += p1.vx;
        p1.y += p1.vy;


		//Checking for canvas bounds
        if (boundsToggle) {
            if (p1.x < 0) { p1.vx *= -1; p1.x = 0; }
            if (p1.x > boundWidth) { p1.vx *= -1; p1.x = boundWidth; }
            if (p1.y < 0) { p1.vy *= -1; p1.y = 0; }
            if (p1.y > boundHeight) { p1.vy *= -1; p1.y = boundHeight; }

        }
        (*Group1)[i] = p1;
    }

}

//Generate new sets of points
void ofApp::restart() {
    if (numberSliderG > 0) { green = CreatePoints(numberSliderG, 100, 250, 10); }
    if (numberSliderR > 0) { red = CreatePoints(numberSliderR, 250, 10, 100); }
    if (numberSliderW > 0) { white = CreatePoints(numberSliderW, 250, 250, 250); }
    if (numberSliderB > 0) { blue = CreatePoints(numberSliderB, 100, 100, 250); }
}


//Generate initial simulation parameters
void ofApp::random() {
    // GREEN
    //numberSliderG = RandomFloat(0, 3000);
    powerSliderGG = RandomFloat(-100, 100) * forceVariance;
    powerSliderGR = RandomFloat(-100, 100) * forceVariance;
    powerSliderGW = RandomFloat(-100, 100) * forceVariance;
    powerSliderGB = RandomFloat(-100, 100) * forceVariance;

    vSliderGG = RandomFloat(10, 500) * radiusVariance;
    vSliderGR = RandomFloat(10, 500) * radiusVariance;
    vSliderGW = RandomFloat(10, 500) * radiusVariance;
    vSliderGB = RandomFloat(10, 500) * radiusVariance;

    // RED
    //numberSliderR = RandomFloat(0, 3000);
    powerSliderRR = RandomFloat(-100, 100) * forceVariance;
    powerSliderRG = RandomFloat(-100, 100) * forceVariance;
    powerSliderRW = RandomFloat(-100, 100) * forceVariance;
    powerSliderRB = RandomFloat(-100, 100) * forceVariance;

    vSliderRG = RandomFloat(10, 500) * radiusVariance;
    vSliderRR = RandomFloat(10, 500) * radiusVariance;
    vSliderRW = RandomFloat(10, 500) * radiusVariance;
    vSliderRB = RandomFloat(10, 500) * radiusVariance;

    // WHITE
   // numberSliderW = RandomFloat(0, 3000);
    powerSliderWW = RandomFloat(-100, 100) * forceVariance;
    powerSliderWR = RandomFloat(-100, 100) * forceVariance;
    powerSliderWG = RandomFloat(-100, 100) * forceVariance;
    powerSliderWB = RandomFloat(-100, 100) * forceVariance;

    vSliderWG = RandomFloat(10, 500) * radiusVariance;
    vSliderWR = RandomFloat(10, 500) * radiusVariance;
    vSliderWW = RandomFloat(10, 500) * radiusVariance;
    vSliderWB = RandomFloat(10, 500) * radiusVariance;

    // BLUE
    //numberSliderB = RandomFloat(0, 3000);
    powerSliderBB = RandomFloat(-100, 100) * forceVariance;
    powerSliderBW = RandomFloat(-100, 100) * forceVariance;
    powerSliderBR = RandomFloat(-100, 100) * forceVariance;
    powerSliderBG = RandomFloat(-100, 100) * forceVariance;

    vSliderBG = RandomFloat(10, 500) * radiusVariance;
    vSliderBR = RandomFloat(10, 500) * radiusVariance;
    vSliderBW = RandomFloat(10, 500) * radiusVariance;
    vSliderBB = RandomFloat(10, 500) * radiusVariance;
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
        static_cast<float>(numberSliderG), static_cast<float>(numberSliderR), static_cast<float>(numberSliderW), static_cast<float>(numberSliderB),
        viscoSlider
    };

    std::string save_path = "";
    ofFileDialogResult result = ofSystemSaveDialog("model.txt", "Save");
    if (result.bSuccess) {
        save_path = result.getPath();
    }
    else {
        ofSystemAlertDialog("Could not Save Model!");
    }
    ofstream myfile(save_path);
    if (myfile.is_open()) {
        for (int i = 0; i < settings.size(); i++) {
            myfile << settings[i] << " ";
        }
        myfile.close();
        std::cout << "file saved successfully";
    }
    else {
        std::cout << "unable to save file!";
    }
    
}

// Dialog gui tested on windows machine only. Not sure if it works on Mac or Linux too.
void ofApp::loadSettings()
{
    std::string load_path;
    std::string text;
    ofFileDialogResult result = ofSystemLoadDialog("Load file", false, load_path);
    if (result.bSuccess) {
        load_path = result.getPath();
        std::ifstream t(load_path);
        std::stringstream loaded;
        loaded << t.rdbuf();
        text = loaded.str();
    }
    else {
        ofSystemAlertDialog("Could not Load the File!");
    }

    // split text by space and convert them to floats
    string word;
    std::vector<float> p;
    for (auto x : text){
        if (x == ' '){
            p.push_back(std::stof(word));
            word = "";
        }else {
            word = word + x;
        }
    }

    if (p.size() < 37) {
        // better checks needed
        ofSystemAlertDialog("Could not read the file!");
    }
    else {
        powerSliderGG = p[0]; powerSliderGR = p[1]; powerSliderGW = p[2]; powerSliderGB = p[3];
        vSliderGG = p[4]; vSliderGR = p[5]; vSliderGW = p[6]; vSliderGB = p[7];
        powerSliderRG = p[8]; powerSliderRR = p[9]; powerSliderRW = p[10]; powerSliderRB = p[11];
        vSliderRG = p[12]; vSliderRR = p[13]; vSliderRW = p[14]; vSliderRB = p[15];
        powerSliderWG = p[16]; powerSliderWR = p[17]; powerSliderWW = p[18]; powerSliderWB = p[19];
        vSliderWG = p[20]; vSliderWR = p[21]; vSliderWW = p[22]; vSliderWB = p[23];
        powerSliderBG = p[24]; powerSliderBR = p[25]; powerSliderBW = p[26]; powerSliderBB = p[27];
        vSliderBG = p[28]; vSliderBR = p[29]; vSliderBW = p[30]; vSliderBB = p[31];
        numberSliderG = static_cast<int>(p[32]); numberSliderR = static_cast<int>(p[33]); numberSliderW = static_cast<int>(p[34]); numberSliderB = static_cast<int>(p[35]);
        viscoSlider = p[36];
    }
    restart();
}





//------------------------------GUI initialization------------------------------
void ofApp::setup(){
    lastTime = clock();
    ofSetWindowTitle("Particle Life - www.brainxyz.com");
    // Interface
    gui.setup("Settings");
    gui.loadFont("Arial", 12);
    gui.setWidthElements(300.0f);

    gui.add(resetButton.setup("START/RESTART"));
    gui.add(randomChoice.setup("Randomize"));
    gui.add(save.setup("Save Model"));
    gui.add(load.setup("Load Model"));
    gui.add(viscoSlider.setup("Viscosity/Friction", viscosity, 0, 1));
    gui.add(gravitySlider.setup("Gravity", gravity, 0, 1));
    //gui.add(labelG.setup("GREEN:", "-"));
    gui.add(numberSliderG.setup("GREEN:", pnumberSliderG, 0, 3000));
    gui.add(powerSliderGG.setup("green x green:", ppowerSliderGG, -100, 100));
    gui.add(powerSliderGR.setup("green x red:", ppowerSliderGR, -100, 100));
    gui.add(powerSliderGW.setup("green x white:", ppowerSliderGW, -100, 100));
    gui.add(powerSliderGB.setup("green x blue:", ppowerSliderGB, -100, 100));

    gui.add(vSliderGG.setup("radius g x g:", pvSliderGG, 10, 500));
    gui.add(vSliderGR.setup("radius g x r:", pvSliderGR, 10, 500));
    gui.add(vSliderGW.setup("radius g x w:", pvSliderGW, 10, 500));
    gui.add(vSliderGB.setup("radius g x b:", pvSliderGB, 10, 500));

    //gui.add(labelR.setup("RED:", "-"));
    gui.add(numberSliderR.setup("RED:", pnumberSliderR, 0, 3000));
    gui.add(powerSliderRR.setup("red x red:", ppowerSliderRR, -100, 100));
    gui.add(powerSliderRG.setup("red x green:", ppowerSliderRG, -100, 100));
    gui.add(powerSliderRW.setup("red x white:", ppowerSliderRW, -100, 100));
    gui.add(powerSliderRB.setup("red x blue:", ppowerSliderRB, -100, 100));

    gui.add(vSliderRG.setup("radius r x g:", pvSliderRG, 10, 500));
    gui.add(vSliderRR.setup("radius r x r:", pvSliderRR, 10, 500));
    gui.add(vSliderRW.setup("radius r x w:", pvSliderRW, 10, 500));
    gui.add(vSliderRB.setup("radius r x b:", pvSliderRB, 10, 500));

    //gui.add(labelW.setup("WHITE:", "-"));
    gui.add(numberSliderW.setup("WHITE:", pnumberSliderW, 0, 3000));
    gui.add(powerSliderWW.setup("white x white:", ppowerSliderWW, -100, 100));
    gui.add(powerSliderWR.setup("white x red:", ppowerSliderWR, -100, 100));
    gui.add(powerSliderWG.setup("white x green:", ppowerSliderWG, -100, 100));
    gui.add(powerSliderWB.setup("white x blue:", ppowerSliderWB, -100, 100));

    gui.add(vSliderWG.setup("radius w x g:", pvSliderWG, 10, 500));
    gui.add(vSliderWR.setup("radius w x r:", pvSliderWR, 10, 500));
    gui.add(vSliderWW.setup("radius w x w:", pvSliderWW, 10, 500));
    gui.add(vSliderWB.setup("radius w x b:", pvSliderWB, 10, 500));

    //gui.add(labelB.setup("BLUE:", "-"));
    gui.add(numberSliderB.setup("BLUE:", pnumberSliderB, 0, 3000));
    gui.add(powerSliderBB.setup("blue x blue:", ppowerSliderBB, -100, 100));
    gui.add(powerSliderBW.setup("blue x white:", ppowerSliderBW, -100, 100));
    gui.add(powerSliderBR.setup("blue x red:", ppowerSliderBR, -100, 100));
    gui.add(powerSliderBG.setup("blue x green:", ppowerSliderBG, -100, 100));

    gui.add(vSliderBG.setup("radius b x g:", pvSliderBG, 10, 500));
    gui.add(vSliderBR.setup("radius b x r:", pvSliderBR, 10, 500));
    gui.add(vSliderBW.setup("radius b x w:", pvSliderBW, 10, 500));
    gui.add(vSliderBB.setup("radius b x b:", pvSliderBB, 10, 500));
    gui.add(boundsToggle.setup("Bounded", true));
    gui.add(modelToggle.setup("Show Model", false));
    gui.add(fps.setup("FPS", "0"));

    restart();
}

//------------------------------Update simulation with sliders values------------------------------
void ofApp::update(){
    viscosity = viscoSlider;
    gravity = gravitySlider;
    if (numberSliderG > 0) {
        interaction(&green, &green, powerSliderGG, vSliderGG);
        interaction(&green, &red, powerSliderGR, vSliderGR);
        interaction(&green, &white, powerSliderGW, vSliderGW);
        interaction(&green, &blue, powerSliderGB, vSliderGB);
    }

    if (numberSliderR > 0) {
        interaction(&red, &red, powerSliderRR, vSliderRR);
        interaction(&red, &green, powerSliderRG, vSliderRG);
        interaction(&red, &white, powerSliderRW, vSliderRW);
        interaction(&red, &blue, powerSliderRB, vSliderRB);
    }
    if (numberSliderW > 0) {
        interaction(&white, &white, powerSliderWW, vSliderWW);
        interaction(&white, &green, powerSliderWG, vSliderWG);
        interaction(&white, &red, powerSliderWR, vSliderWR);
        interaction(&white, &blue, powerSliderWB, vSliderWB);
    }
    if (numberSliderB > 0) {
        interaction(&blue, &white, powerSliderBW, vSliderBW);
        interaction(&blue, &green, powerSliderBG, vSliderBG);
        interaction(&blue, &red, powerSliderBR, vSliderBR);
        interaction(&blue, &blue, powerSliderBB, vSliderBB);
    }
    
    if (save) { saveSettings(); }
    if (load) { loadSettings(); }
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);  // Clear the screen with a black color

    //fps counter
    cntFps++;
    now = clock();
    delta = now - lastTime;

	//Time step
    if (delta >= 1000)
    {
        lastTime = now;

        fps.setup("FPS", to_string(static_cast<int>((1000 / static_cast<double>(delta)) * cntFps)));
        cntFps = 0;
    }

	//Check for GUI interaction
    if (resetButton) { restart(); }
    if (randomChoice) { random(); restart(); }
    if (numberSliderW > 0) { Draw(&white); }
    if (numberSliderR > 0) { Draw(&red); }
    if (numberSliderG > 0) { Draw(&green); }
    if (numberSliderB > 0) { Draw(&blue); }


	//Draw GUI
    if (modelToggle == true) {

        ofSetColor(0, 0, 0);
        ofDrawCircle(xshift, yshift, 150);

        ofSetLineWidth(5);
        ofSetColor(150 - powerSliderGR, 150 + powerSliderGR, 150);
        ofDrawLine(p1x, p1y - 10, p2x, p2y - 10);
        ofSetColor(150 - powerSliderRG, 150 + powerSliderRG, 150);
        ofDrawLine(p1x, p1y + 10, p2x, p2y + 10);
        ofSetColor(150 - powerSliderGW, 150 + powerSliderGW, 150);
        ofDrawLine(p3x, p3y - 10, p1x, p1y - 10);
        ofSetColor(150 - powerSliderWG, 150 + powerSliderWG, 150);
        ofDrawLine(p3x, p3y + 10, p1x, p1y + 10);

        ofSetColor(150 - powerSliderGB, 150 + powerSliderGB, 150);
        ofDrawLine(p4x - 10, p4y, p1x - 10, p1y);
        ofSetColor(150 - powerSliderBG, 150 + powerSliderBG, 150);
        ofDrawLine(p4x + 10, p4y, p1x + 10, p1y);

        ofSetColor(150 - powerSliderRW, 150 + powerSliderRW, 150);
        ofDrawLine(p2x - 10, p2y, p3x - 10, p3y);
        ofSetColor(150 - powerSliderWR, 150 + powerSliderWR, 150);
        ofDrawLine(p2x + 10, p2y, p3x + 10, p3y);

        ofSetColor(150 - powerSliderRB, 150 + powerSliderRB, 150);
        ofDrawLine(p2x, p2y - 10, p4x, p4y - 10);
        ofSetColor(150 - powerSliderBR, 150 + powerSliderBR, 150);
        ofDrawLine(p2x, p2y + 10, p4x, p4y + 10);

        ofSetColor(150 - powerSliderWB, 150 + powerSliderWB, 150);
        ofDrawLine(p3x, p3y - 10, p4x, p4y - 10);
        ofSetColor(150 - powerSliderBW, 150 + powerSliderBW, 150);
        ofDrawLine(p3x, p3y + 10, p4x, p4y + 10);

        ofNoFill();
        ofSetColor(150 - powerSliderGG, 150 + powerSliderGG, 150);
        ofDrawCircle(p1x - 20, p1y - 20, rr + 20);
        ofSetColor(150 - powerSliderRR, 150 + powerSliderRR, 150);
        ofDrawCircle(p2x + 20, p2y - 20, rr + 20);
        ofSetColor(150 - powerSliderWW, 150 + powerSliderWW, 150);
        ofDrawCircle(p3x + 20, p3y + 20, rr + 20);
        ofSetColor(150 - powerSliderBB, 150 + powerSliderBB, 150);
        ofDrawCircle(p4x - 20, p4y + 20, rr + 20);

        ofFill();
        ofSetColor(100, 250, 10);
        ofDrawCircle(p1x, p1y, rr);
        ofSetColor(250, 10, 100);
        ofDrawCircle(p2x, p2y, rr);
        ofSetColor(250, 250, 250);
        ofDrawCircle(p3x, p3y, rr);
        ofSetColor(100, 100, 250);
        ofDrawCircle(p4x, p4y, rr);
    }
    gui.draw();
}

