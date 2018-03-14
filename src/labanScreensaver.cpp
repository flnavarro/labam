//
//  labanSavescreen.cpp
//  emptyExample
//
//  Created by Felipe L. Navarro on 12/03/18.
//
//

#include "labanScreensaver.h"

//--------------------------------------------------------------
void labanScreensaver::setup() {
    
    // Debug
    debug = true;
    
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
    
    // Colors
    labanColor[0] = ofColor(13, 20, 13);
    labanColor[1] = ofColor(16, 34, 38);
    labanColor[2] = ofColor(104, 140, 137);
    labanColor[3] = ofColor(129, 191, 181);
    labanColor[4] = ofColor(73, 73, 73);
    labanColor[5] = ofColor(241, 242, 242);
    
    // Opciones
    fadeoutIsActive = false; // fade out en la parte baja de la pantalla
    alphaDecrease = 5; // cuánto alpha quitamos al haber una mano encima de la img
    minSpeed = 1.f; // velocidad mínima de caída de símbolo (pixels por frame)
    maxSpeed = 2.5f; // velocidad máxima de caída de símbolo
    nColumns = 6; // número de columnas
    nSymbolsPerCol = 10; // tamaño vector de símbolos para cada columna
    screenHeight = ofGetHeight();
    colWidth = int(float(ofGetWidth())/float(nColumns));
    marginWidth = colWidth * 0.1f;
    marginHeight = marginWidth;
    imgWidth = colWidth - marginWidth * 2;
    // Crear vectores
    for(int i=0; i<nColumns; i++){
        float speed = ofRandom(minSpeed, maxSpeed);
        fallingSpeed.push_back(speed);
        vector<ofImage*> oneColumn;
        vector<ofPoint> oneColumnPos;
        vector<ofVec2f> oneColumnSizes;
        vector<int> oneColumnIdx;
        vector<int> oneColumnAlpha;
        vector<ofColor> oneColumnColor;
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
            oneColumnAlpha.push_back(255);
            // Assign Color
            int colorChoice = ofRandom(0, 5);
            oneColumnColor.push_back(labanColor[colorChoice]);
        }
        colSymbols.push_back(oneColumn);
        symbolIdx.push_back(oneColumnIdx);
        symbolPos.push_back(oneColumnPos);
        symbolSize.push_back(oneColumnSizes);
        symbolAlpha.push_back(oneColumnAlpha);
        symbolColor.push_back(oneColumnColor);
    }
}

//--------------------------------------------------------------
void labanScreensaver::resizeAll() {
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
            symbolAlpha[i][j] = 255;
        }
    }
}

//--------------------------------------------------------------
void labanScreensaver::update(ofPoint handPos[2]) {
    // auto detect changes in fullscreen
    if(ofGetWidth() != screenWidth){
        resizeAll();
    }
    
    for(int i=0; i<symbolPos.size(); i++){
        for(int j=0; j<symbolPos[i].size(); j++){
            
            // Changing position (falling)
            symbolPos[i][j].y += fallingSpeed[i];
            
            // Hands interaction
            for(int k=0; k<2; k++){
                if( ofRectangle(symbolPos[i][j].x, symbolPos[i][j].y,
                                symbolSize[i][j].x, symbolSize[i][j].y).inside(handPos[k].x, handPos[k].y) ){
                    symbolAlpha[i][j] -= alphaDecrease;
                }
            }
            
            // Fadeout effect (if active)
            if( symbolPos[i][j].y > 3*screenHeight/4 &&
                      symbolPos[i][j].y < screenHeight &&
                      fadeoutIsActive){
                if(symbolAlpha[i][j] > 50){
                    symbolAlpha[i][j] = ofMap(symbolPos[i][j].y,
                                              3*screenHeight/4, screenHeight,
                                              symbolAlpha[i][j], 50);
                }
            }
            
            // Reload symbol when finished falling
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
                // Reset Alpha
                symbolAlpha[i][j] = 255;
                // Assign New Color
                int colorChoice = ofRandom(0, 5);
                symbolColor[i][j] = labanColor[colorChoice];
            }
        }
    }
    
    screenWidth = ofGetWidth();
}

//--------------------------------------------------------------
void labanScreensaver::draw() {
    ofBackgroundHex(0xffffff);
    
    for(int i=0; i<colSymbols.size(); i++){
        for(int j=0; j<colSymbols[i].size(); j++){
            ofPushStyle();
            // Draw symbols
            ofSetColor(symbolColor[i][j], symbolAlpha[i][j]);
            colSymbols[i][j]->draw(symbolPos[i][j].x, symbolPos[i][j].y,
                                   symbolSize[i][j].x, symbolSize[i][j].y);
            ofPopStyle();
        }
    }
    
    if(debug){
        ofPushStyle();
        string info = "";
        info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
        ofSetHexColor(0x444342);
        ofDrawBitmapString(info, 30, 30);
        ofPopStyle();
    }
}