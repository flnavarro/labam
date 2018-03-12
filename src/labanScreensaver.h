#pragma once
#include "ofMain.h"

// -------------------------------------------------

class labanScreensaver : public ofBaseApp {
    
    public:
    
        void setup();
        void update(ofPoint handPos[2]);
        void draw();
    
        bool debug;
    
        void resizeAll();
        int screenWidth;
    
        vector<ofImage*> labanSymbol;
        int nSymbols;
    
        int nColumns;
        int screenHeight;
        int colWidth;
        float marginWidth;
        float marginHeight;
        float imgWidth;
    
        int nSymbolsPerCol;
        bool fadeoutIsActive;
        int alphaDecrease;
        float minSpeed;
        float maxSpeed;
        vector<float> fallingSpeed; // pixel per frame
        vector<vector<ofImage*>> colSymbols;
        vector<vector<int>> symbolIdx;
        vector<vector<ofPoint>> symbolPos;
        vector<vector<ofVec2f>> symbolSize;
        vector<vector<int>> symbolAlpha;
};