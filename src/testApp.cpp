#include "testApp.h"

static bool removeShapeOffScreen(shared_ptr<ofxBox2dBaseShape> shape) {
    if (!ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape.get()->getPosition())) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void testApp::setup() {
    
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	box2d.init();
	box2d.setGravity(0, 10);
	// box2d.createBounds();
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
    
    ground.setPhysics(0, 0, 0);
    ground.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()+200, 400);
    groundMesh.setUsage(GL_STATIC_DRAW);
    groundMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for(int i=0; i<100; i++) {
        float t = ofMap(i, 0, 99, 0.0, TWO_PI);
        ofVec3f v(ofGetWidth()/2, ofGetHeight()+200);
        groundMesh.addVertex(v);
        v.x += cos(t)*400;
        v.y += sin(t)*400;
        groundMesh.addVertex(v);
    }
}

//--------------------------------------------------------------
void testApp::update() {
    ofRemove(circles, removeShapeOffScreen);
    ofRemove(boxes, removeShapeOffScreen);
    
	box2d.update();
    
    if(geneCounter < 20){
        geneCounter++;
    } else {
        generateSymbol();
        geneCounter = 0;
    }
}


void testApp::generateSymbol() {
    float r = ofRandom(20, 30);
    circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
    circles.back().get()->setPhysics(3.0, 0.53, 0.1);
    float x = ofRandom(0, ofGetWidth());
    float y = ofRandom(0, 0);
    circles.back().get()->setup(box2d.getWorld(), x, y, r);
    images.push_back(ofImage());
    int randChoice = ofRandom(0, 3);
    string path;
    if(randChoice == 0){
        path = "symbols/ForwardLow.png";
    } else if (randChoice == 1){
        path = "symbols/ForwardMiddle.png";
    } else if (randChoice == 2){
        path = "symbols/SideLow.png";
    }
    images[images.size()-1].load(path);
}


//--------------------------------------------------------------
void testApp::draw() {
	
	
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		// circles[i].get()->draw();
        float posX = circles[i].get()->getPosition().x;
        float posY = circles[i].get()->getPosition().y;
        float rot = circles[i].get()->getRotation();
        float rad = circles[i].get()->getRadius();
        ofPushMatrix();
        ofTranslate(posX, posY);
        ofRotate(rot);
        images[i].setAnchorPercent(0.5, 0.5);
        ofSetColor(255);
        images[i].draw(0, 0, rad*2, rad*2);
        ofPopMatrix();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].get()->draw();
	}

    ofSetColor(255, 100);
    groundMesh.draw();
    
	// draw the ground
	// box2d.drawGround();
	
	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'c') {
        generateSymbol();
	}
	
	if(key == 'b') {
		float w = ofRandom(4, 20);
		float h = ofRandom(4, 20);
		boxes.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
		boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
		boxes.back().get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
	}
	
	if(key == 't') ofToggleFullscreen();
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

