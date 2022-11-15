
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(1600,900,OF_WINDOW);
	ofGLWindowSettings s;
	//s.setGLVersion(4, 3);
	s.setSize(1600, 900);
	//s.setPosition(glm::vec2(0, 0));
	ofCreateWindow(s);

	ofRunApp(new ofApp());

}
