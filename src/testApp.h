#pragma once
#include "ofMain.h"
#include "labanScreensaver.h"

// -------------------------------------------------

class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
    
    bool fullscreen;
    
    labanScreensaver labanScreensaver;
    
    ofPoint mousePos[2];
};

