// Author:  Hunar Ahmad Abdulrahman @ brainxyz.com 2022

#include "ofApp.h"

#include <iostream>
#include <vector>
#include <omp.h>

// parameters for image positions
float xshift = 510;
float yshift = 80;
float anchor = 0;
float length = 100;
float p1x = anchor+xshift;
float p1y = anchor+yshift;
float p2x = anchor+length+xshift;
float p2y = anchor+yshift;
float p3x = anchor+length+xshift;
float p3y = anchor+length+yshift;
float p4x = anchor + xshift;
float p4y = anchor + length + yshift;
float rr = 15;
int countThresh = 0;

std::vector<Point> green;
std::vector<Point> red;
std::vector<Point> white;
std::vector<Point> blue;

float RandomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
void Draw(std::vector<Point>* points) {
    for (int i = 0; i < points->size(); i++) {
        (* points)[i].Draw();
    }
}
int Random(int mn, int mx) { return rand() % (mx - mn) + mn; }

std::vector<Point> CreatePoints(int num, int r, int g, int b) {
    std::vector<Point> points;
    for (int i = 0; i < num; i++) {
        int x = Random(900, 1700);
        int y = Random(100, 900);
        points.push_back(Point(x, y, r, g, b));
    }
    return points;
}

// Interaction between 2 particle groups
void ofApp::Interaction(std::vector<Point>* Group1, std::vector<Point>* Group2, float G, float radius) {
    
    float g = G / -100;

    omp_set_num_threads(4);
    #pragma omp parallel for 
    for (int i = 0; i < Group1->size(); i++) {
        auto p1 = (*Group1)[i];
        float fx = 0;
        float fy = 0;
        for (int j = 0; j < Group2->size(); j++) {
            auto p2 = (*Group2)[j];
            auto dx = p1.x - p2.x;
            auto dy = p1.y - p2.y;
            auto r = std::sqrt(dx * dx + dy * dy);
            if (r < radius && r > 0) {
                fx += (dx / r);
                fy += (dy / r);
            }
        }

        p1.vx = (p1.vx + (fx * g)) * 0.5;
        p1.vy = (p1.vy + (fy * g)) * 0.5;
        p1.x += p1.vx;
        p1.y += p1.vy;

        if (boundsToggle) {
            if ((p1.x >= 1920 - 10) || (p1.x <= 550 + 10)) {p1.vx *= -1;}
            if ((p1.y >= 1024 - 10) || (p1.y <= 0 + 10)) {p1.vy *= -1;}
        }

        (*Group1)[i] = p1;
    }
   
}

void ofApp::restart() {
    if (numberSliderG > 0) { green = CreatePoints(numberSliderR, 100, 250, 10); }
    if (numberSliderR > 0) { red = CreatePoints(numberSliderG, 250, 10, 100); }
    if (numberSliderW > 0) { white = CreatePoints(numberSliderW, 250, 250, 250); }
    if (numberSliderB > 0) { blue = CreatePoints(numberSliderB, 100, 100, 250); }
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Particle Life - www.brainxyz.com");
    // Interface
    gui.setup("Settings");
    gui.loadFont("Arial", 16);
    gui.setWidthElements(400.0f);

    gui.add(resetButton.setup("START/RESET"));
    gui.add(boundsToggle.setup("Bounded", true));
    gui.add(modelToggle.setup("Show Model", true));
    gui.add(labelG.setup("GREEN:", "-"));
    gui.add(numberSliderG.setup("number:", pnumberSliderG, 0, 3000));
    gui.add(powerSliderGG.setup("green x green:", ppowerSliderGG, -100, 100));
    gui.add(powerSliderGR.setup("green x red:", ppowerSliderGR, -100, 100));
    gui.add(powerSliderGW.setup("green x white:", ppowerSliderGW, -100, 100));
    gui.add(powerSliderGB.setup("green x blue:", ppowerSliderGB, -100, 100));

    gui.add(vSliderGG.setup("radius g x g:", pvSliderGG, 10, 500));
    gui.add(vSliderGR.setup("radius g x r:", pvSliderGR, 10, 500));
    gui.add(vSliderGW.setup("radius g x w:", pvSliderGW, 10, 500));
    gui.add(vSliderGB.setup("radius g x b:", pvSliderGB, 10, 500));

    gui.add(labelR.setup("RED:", "-"));
    gui.add(numberSliderR.setup("number:", pnumberSliderR, 0, 3000));
    gui.add(powerSliderRR.setup("red x red:", ppowerSliderRR, -100, 100));
    gui.add(powerSliderRG.setup("red x green:", ppowerSliderRG, -100, 100));
    gui.add(powerSliderRW.setup("red x white:", ppowerSliderRW, -100, 100));
    gui.add(powerSliderRB.setup("red x blue:", ppowerSliderRB, -100, 100));

    gui.add(vSliderRG.setup("radius r x g:", pvSliderRG, 10, 500));
    gui.add(vSliderRR.setup("radius r x r:", pvSliderRR, 10, 500));
    gui.add(vSliderRW.setup("radius r x w:", pvSliderRW, 10, 500));
    gui.add(vSliderRB.setup("radius r x b:", pvSliderRB, 10, 500));

    gui.add(labelW.setup("WHITE:", "-"));
    gui.add(numberSliderW.setup("number:", pnumberSliderW, 0, 3000));
    gui.add(powerSliderWW.setup("white x white:", ppowerSliderWW, -100, 100));
    gui.add(powerSliderWR.setup("white x red:", ppowerSliderWR, -100, 100));
    gui.add(powerSliderWG.setup("white x green:", ppowerSliderWG, -100, 100));
    gui.add(powerSliderWB.setup("white x blue:", ppowerSliderWB, -100, 100));

    gui.add(vSliderWG.setup("radius w x g:", pvSliderWG, 10, 500));
    gui.add(vSliderWR.setup("radius w x r:", pvSliderWR, 10, 500));
    gui.add(vSliderWW.setup("radius w x w:", pvSliderWW, 10, 500));
    gui.add(vSliderWB.setup("radius w x b:", pvSliderWB, 10, 500));

    gui.add(labelB.setup("BLUE:", "-"));
    gui.add(numberSliderB.setup("number:", pnumberSliderB, 0, 3000));
    gui.add(powerSliderBB.setup("blue x blue:", ppowerSliderBB, -100, 100));
    gui.add(powerSliderBW.setup("blue x white:", ppowerSliderBW, -100, 100));
    gui.add(powerSliderBR.setup("blue x red:", ppowerSliderBR, -100, 100));
    gui.add(powerSliderBG.setup("blue x green:", ppowerSliderBG, -100, 100));

    gui.add(vSliderBG.setup("radius b x g:", pvSliderBG, 10, 500));
    gui.add(vSliderBR.setup("radius b x r:", pvSliderBR, 10, 500));
    gui.add(vSliderBW.setup("radius b x w:", pvSliderBW, 10, 500));
    gui.add(vSliderBB.setup("radius b x b:", pvSliderBB, 10, 500));
    gui.add(aboutL3.setup("Info:", "www.brainxyz.com "));

    restart();
}

//--------------------------------------------------------------
void ofApp::update(){

    if (numberSliderG > 0) {
        Interaction(&green, &green, powerSliderGG, vSliderGG);
        Interaction(&green, &red, powerSliderGR, vSliderGR);
        Interaction(&green, &white, powerSliderGW, vSliderGW);
        Interaction(&green, &blue, powerSliderGB, vSliderGB);
    }

    if (numberSliderR > 0) {
        Interaction(&red, &red, powerSliderRR, vSliderRR);
        Interaction(&red, &green, powerSliderRG, vSliderRG);
        Interaction(&red, &white, powerSliderRW, vSliderRW);
        Interaction(&red, &blue, powerSliderRB, vSliderRB);
    }
    if (numberSliderW > 0) {
        Interaction(&white, &white, powerSliderWW, vSliderWW);
        Interaction(&white, &green, powerSliderWG, vSliderWG);
        Interaction(&white, &red, powerSliderWR, vSliderWR);
        Interaction(&white, &blue, powerSliderWB, vSliderWB);
    }
    if (numberSliderB > 0) {
        Interaction(&blue, &white, powerSliderBW, vSliderBW);
        Interaction(&blue, &green, powerSliderBG, vSliderBG);
        Interaction(&blue, &red, powerSliderBR, vSliderBR);
        Interaction(&blue, &blue, powerSliderBB, vSliderBB);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);  // Clear the screen with a black color

    if (resetButton) { restart(); }
    if (numberSliderW > 0) { Draw(&white); }
    if (numberSliderR > 0) { Draw(&red); }
    if (numberSliderG > 0) { Draw(&green); }
    if (numberSliderB > 0) { Draw(&blue); }

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

