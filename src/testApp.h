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
    
    void update2();
    void draw2();
    
    void resizeAll();
    
    void generateSymbol();
    void drawSymbols();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
    int disW, disH;
    bool fullscreen;

    
    vector <bool> insertNewImage;
    bool maxImgReached;
    vector<ofImage> colImgs[5];
    vector<ofPoint> imgPos[5];
    
        
    vector <ofImage> images;
    map<shared_ptr<ofxBox2dCircle>, shared_ptr<ofImage>> mappedImgs;
    float scaleImage;
    int nDivisions;
    int screenDiv;
    int colChoice;

    int geneCounter;
    
    // ----------------- //
    // ---- PHYSICS ---- //
    void setupPhysics();
    void updatePhysics();
    
    ofxBox2d box2d;
    vector <shared_ptr<ofxBox2dCircle>>	circles;
    ofPolyline groundLine;
    ofxBox2dEdge groundEdge;
    
    
    // ----------------- //
    // ---- OPEN CV ---- //
    void setupCV();
    void updateCV();
    void drawCV();
    
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

