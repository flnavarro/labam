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
	// ofBackgroundHex(0xa4c1b8);
    ofBackgroundHex(0xffffff);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	box2d.init();
	box2d.setGravity(0, 2);
	//box2d.createBounds();
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
    
    
    // ---- OPEN CV ---- //
    
    #ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.setup(320, 240);
    #else
        vidPlayer.load("test_video/fingers.mov");
        vidPlayer.play();
        vidPlayer.setLoopState(OF_LOOP_NORMAL);
    #endif
    
    disW = 320;
    disH = 240;
//    disW = 600;
//    disH = 900;
    
    colorImg.allocate(disW, disH);
    grayImage.allocate(disW, disH);
    grayBg.allocate(disW, disH);
    grayDiff.allocate(disW, disH);
    bLearnBackground = true;
    threshold = 80;
    
    screenDiv = int(float(ofGetWidth())/float(5));
}

//--------------------------------------------------------------
void testApp::update() {
    //ofRemove(circles, removeShapeOffScreen);
    //ofRemove(boxes, removeShapeOffScreen);
    
	box2d.update();
    
    if(geneCounter < 10){
        geneCounter++;
    } else {
        generateSymbol();
        geneCounter = 0;
    }
    
    // ---- OPEN CV ---- //
    updateCV();
}


void testApp::updateCV(){
    bool bNewFrame = false;
    
    #ifdef _USE_LIVE_VIDEO
        vidGrabber.update();
        bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
    #endif
    
    if (bNewFrame){
        
        #ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels());
        #else
            colorImg.setFromPixels(vidPlayer.getPixels());
        #endif
        
        grayImage = colorImg;
        if (bLearnBackground == true){
            grayBg = grayImage;
            bLearnBackground = false;
        }
        
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);
        
        contourFinder.findContours(grayDiff, 20, (disW*disH)/3, 10, true);
        
        //groundMesh.clear();
//        float scaleX, scaleY;
//        scaleX = ofGetWidth()/600;
//        scaleY = ofGetHeight()/900;
        groundLine.clear();
        if (contourFinder.nBlobs > 0){
            for(int i=0; i<contourFinder.blobs[0].nPts; i++) {
                float posX, posY;
                float t = ofMap(i, 0, 99, 0.0, TWO_PI);
                posX = (contourFinder.blobs[0].pts[i].x - 50) * 3.75f;
                posY = (contourFinder.blobs[0].pts[i].y) * 3.75f;
                ofVec3f v(posX, posY);
                groundLine.addVertex(v);
                //groundMesh.addVertex(v);
            }
        }
        
        groundEdge.clear();
        //groundEdge.addVertexes(groundLine);
        groundEdge.create(box2d.getWorld());
    }
}


void testApp::generateSymbol() {
    int imgChoice = ofRandom(0, 5);
    string path;
    float screenPos;
    if(imgChoice == 0){
        path = "symbols/00.png";
    } else if (imgChoice == 1){
        path = "symbols/01.png";
    } else if (imgChoice == 2){
        path = "symbols/02.png";
    } else if (imgChoice == 3){
        path = "symbols/03.png";
    } else if (imgChoice == 4){
        path = "symbols/04.png";
    }
    if (colChoice < 4){
        colChoice++;
    } else if (colChoice == 4){
        colChoice = 0;
    }
    screenPos = colChoice * screenDiv;
    
    float r = ofRandom(20, 30);
    circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
    circles.back().get()->setPhysics(3.0, 0.53, 0.1);
    float x = ofRandom(0, ofGetWidth());
    float y = ofRandom(0, 0);
    circles.back().get()->setup(box2d.getWorld(), screenPos, -200.f, r);
    
    images.push_back(ofImage());
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
        ofTranslate(posX + images[i].getWidth()/2*scaleImage + 10, posY);
        //ofRotate(rot);
        images[i].setAnchorPercent(0.5, 0.5);
        ofSetColor(255);
        //scaleImage = (screenDiv - 10) / images[i].getWidth();
        images[i].draw(0, 0, images[i].getWidth()*scaleImage, images[i].getHeight()*scaleImage);
        ofPopMatrix();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].get()->draw();
	}

    ofSetColor(255, 100);
    //groundMesh.draw();
    
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
    
    drawCV();
}

void testApp::drawCV(){
    // draw the incoming, the grayscale, the bg and the thresholded difference
    ofSetHexColor(0xffffff);
    //colorImg.draw(20,20);
    //grayImage.draw(360,20);
    //grayBg.draw(20,280);
    //grayDiff.draw(360,280);
    
    // then draw the contours:
    
    ofFill();
    ofSetHexColor(0x333333);
    //ofDrawRectangle(360,540,320,240);
    ofSetHexColor(0x000000);
    
    // we could draw the whole contour finder
    // contourFinder.draw(360,540);
    
    groundEdge.updateShape();
    groundEdge.draw();
    
    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofScale(3.75f, 3.75f);
        //contourFinder.blobs[i].draw(0,0);
        
        // draw over the centroid if the blob is a hole
        ofSetColor(255);
        if(contourFinder.blobs[i].hole){
            ofDrawBitmapString("hole",
                               contourFinder.blobs[i].boundingRect.getCenter().x + 0,
                               contourFinder.blobs[i].boundingRect.getCenter().y + 0);
        }
    }
    
    // finally, a report:
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
    << "press ' ' to capture bg" << endl
    << "threshold " << threshold << " (press: +/-)" << endl
    << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
    ofDrawBitmapString(reportStr.str(), 20, 600);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    if(key == 'f') {
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
    }
	
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
    
    switch (key){
        case ' ':
            bLearnBackground = true;
            break;
        case '+':
            threshold ++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold --;
            if (threshold < 0) threshold = 0;
            break;
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

