#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    fullscreen = false;
    ofSetFullscreen(fullscreen);
    
	ofSetVerticalSync(true);
    ofSetFrameRate(120);
    ofBackgroundHex(0xffffff);
	ofSetLogLevel(OF_LOG_NOTICE);
    
    // --- SAVESCREEN --- //
    setupSavescreen();
}


void testApp::setupSavescreen() {
    
    // Symbols
    string path = "symbols/";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.listDir();
    for(int i=0; i<dir.size(); i++){
        ofImage * symbolImg = new ofImage();
        symbolImg->load(dir.getPath(i));
        labanSymbol.push_back(symbolImg);
    }
    nSymbols = labanSymbol.size();
    
    // Screen Div
    fallingSpeed = 2.5f; // pixel per frame
    nColumns = 6;
    nSymbolsPerCol = 6;
    screenHeight = ofGetHeight();
    colWidth = int(float(ofGetWidth())/float(nColumns));
    marginWidth = colWidth * 0.1f;
    marginHeight = marginWidth;
    imgWidth = colWidth - marginWidth * 2;
    for(int i=0; i<nColumns; i++){
        vector<ofImage*> oneColumn;
        vector<ofPoint> oneColumnPos;
        vector<ofVec2f> oneColumnSizes;
        vector<int> oneColumnIdx;
        int posX = marginWidth + colWidth * i;
        int posY = 0;
        int choice = 0;
        int prevChoice = 0;
        for(int j=0; j<nSymbolsPerCol; j++){
            // Select Image
            while(choice == prevChoice){
                // Don't select same img for 2 consecutive
                choice = ofRandom(0, nSymbols);
            }
            // Assign Img
            ofImage * symbolImg = &(*labanSymbol[choice]);
            // Assign Size
            float scale, width, height;
            scale = imgWidth / symbolImg->getWidth();
            width = symbolImg->getWidth() * scale;
            height = symbolImg->getHeight() * scale;
            oneColumnSizes.push_back(ofVec2f(width, height));
            oneColumn.push_back(symbolImg);
            oneColumnIdx.push_back(choice);
            // Assign Pos
            posY += - marginWidth - height;
            oneColumnPos.push_back(ofPoint(posX, posY));
            // Save choice
            prevChoice = choice;
        }
        colSymbols.push_back(oneColumn);
        symbolIdx.push_back(oneColumnIdx);
        symbolPos.push_back(oneColumnPos);
        symbolSize.push_back(oneColumnSizes);
    }
}

void testApp::resizeAll(){
    screenHeight = ofGetHeight();
    colWidth = int(float(ofGetWidth())/float(nColumns));
    marginWidth = colWidth * 0.1f;
    marginHeight = marginWidth;
    imgWidth = colWidth - marginWidth * 2;
    for(int i=0; i<nColumns; i++){
        int posX = marginWidth + colWidth * i;
        int posY = 0;
        for(int j=0; j<nSymbolsPerCol; j++){
            // Assign Size
            float scale, width, height;
            scale = imgWidth / colSymbols[i][j]->getWidth();
            width = colSymbols[i][j]->getWidth() * scale;
            height = colSymbols[i][j]->getHeight() * scale;
            symbolSize[i][j] = ofVec2f(width, height);
            // Assign Pos
            posY += - marginWidth - height;
            symbolPos[i][j] = ofPoint(posX, posY);
        }
    }
}


//--------------------------------------------------------------
void testApp::update() {

    updateSavescreen();
    
}


void testApp::updateSavescreen() {
    
    for(int i=0; i<symbolPos.size(); i++){
        for(int j=0; j<symbolPos[i].size(); j++){
            symbolPos[i][j].y += fallingSpeed;
            if( symbolPos[i][j].y > screenHeight ){
                // Get previous index
                int prevIdx;
                if(j!=0){
                    prevIdx = j-1;
                } else {
                    prevIdx = symbolPos[i].size() - 1;
                }
                // Make possible choices vector
                vector<int> possibleChoices;
                // Get previous column index image choice
                int prevChoice = symbolIdx[i][prevIdx];
                for(int k=0; k<nSymbols; k++){
                    if( k!=prevChoice && k!=symbolIdx[i][j] ){
                        possibleChoices.push_back(k);
                    }
                }
                int idx_choice = ofRandom(0, possibleChoices.size());
                // Save new index
                symbolIdx[i][j] = possibleChoices[idx_choice];
                // Assign New Image
                colSymbols[i][j] = &(*labanSymbol[symbolIdx[i][j]]);
                // Assign Size
                float scale;
                scale = imgWidth / colSymbols[i][j]->getWidth();
                symbolSize[i][j].x = colSymbols[i][j]->getWidth() * scale;
                symbolSize[i][j].y = colSymbols[i][j]->getHeight() * scale;
                // Assign New Pos
                symbolPos[i][j].y = symbolPos[i][prevIdx].y - symbolSize[i][j].y - marginHeight;

            }
        }
    }
    
}

void testApp::drawSavescreen() {
    for(int i=0; i<colSymbols.size(); i++){
        for(int j=0; j<colSymbols[i].size(); j++){
            if(symbolPos[i][j].y > -symbolSize[i][j].y-marginHeight &&
               symbolPos[i][j].y < ofGetHeight()){
                colSymbols[i][j]->draw(symbolPos[i][j].x, symbolPos[i][j].y,
                                       symbolSize[i][j].x, symbolSize[i][j].y);
            }
        }
    }
    
    ofPushStyle();
    string info = "";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::draw() {

    drawSavescreen();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    if(key == 'f') {
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
        resizeAll();
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

