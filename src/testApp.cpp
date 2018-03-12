#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    fullscreen = false;
    ofSetFullscreen(fullscreen);
    
	ofSetVerticalSync(true);
    ofSetFrameRate(120);
    ofBackgroundHex(0xffffff);
	ofSetLogLevel(OF_LOG_NOTICE);
    
    labanScreensaver.setup();
}


//--------------------------------------------------------------
void testApp::update() {
    labanScreensaver.update(mousePos);
}


//--------------------------------------------------------------
void testApp::draw() {
    labanScreensaver.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    if(key == 'f') {
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
    mousePos[0].x = x;
    mousePos[0].y = y;
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

