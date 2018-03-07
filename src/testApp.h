#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"

// #define _USE_LIVE_VIDEO

// -------------------------------------------------

class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
    
    void updateCV();
    void drawCV();
    void generateSymbol();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
    int disW, disH;
    bool fullscreen;
    
	ofxBox2d                            box2d;			  //	the box2d world
	vector    <shared_ptr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector	  <shared_ptr<ofxBox2dRect> >	boxes;			  //	defalut box2d rects
        
    vector <ofImage> images;
    float scaleImage = 0.25f;
    int screenDiv;
    int colChoice = 0;

    ofPolyline groundLine;
    ofxBox2dEdge groundEdge;

    int geneCounter;
    
    // Open CV
    #ifdef _USE_LIVE_VIDEO
        ofVideoGrabber vidGrabber;
    #else
        ofVideoPlayer vidPlayer;
    #endif
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;
    ofxCvContourFinder contourFinder;
    int threshold;
    bool bLearnBackground;
};

