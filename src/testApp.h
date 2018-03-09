#pragma once
#include "ofMain.h"

#include "ofxBox2d.h"

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
    
    void setupSavescreen();
    void updateSavescreen();
    void drawSavescreen();
    
    void resizeAll();
    
    bool fullscreen;
    
    vector<ofImage*> labanSymbol;
    int nSymbols;
    
    int nColumns;
    int screenHeight;
    int colWidth;
    float marginWidth;
    float marginHeight;
    float imgWidth;
    float fallingSpeed; // pixel per frame

    int nSymbolsPerCol;
    vector<vector<ofImage*>> colSymbols;
    vector<vector<int>> symbolIdx;
    vector<vector<ofPoint>> symbolPos;
    vector<vector<ofVec2f>> symbolSize;
    vector<vector<int>> symbolAlpha;
    
    ofPoint mousePos;

};

