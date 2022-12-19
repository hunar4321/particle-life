
#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings s;
	//s.setGLVersion(4, 6);	// OpenGL 4.6 ( GTX 600 and up )
	s.setSize(1600, 900);
	ofCreateWindow(s);
	ofRunApp(new ofApp());
}
