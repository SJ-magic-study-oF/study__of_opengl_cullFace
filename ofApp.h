/************************************************************
ofVbo
	http://openframeworks.cc/documentation/gl/ofVbo/
************************************************************/
#pragma once


/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxGui.h"

/************************************************************
************************************************************/
class ofApp : public ofBaseApp{
private:
	enum{
		NUM_POINTS = 4,
	};
	
	ofVbo Vbo;
	ofVec3f Verts[NUM_POINTS];
	ofFloatColor Color[NUM_POINTS];
	
	ofEasyCam cam;
	
	void clear_VboSetting_gl();
	
	ofxPanel gui;
	ofxToggle b_Cull;
	ofxToggle b_FrontIsClockWise;
	ofxToggle b_Cull_Front;
	
	

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
};
