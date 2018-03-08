#include "testApp.h"

static bool removeShapeOffScreen(shared_ptr<ofxBox2dBaseShape> shape) {
    if (!ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape.get()->getPosition())) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void testApp::setup() {
    //ofSetFullscreen(true);
    
	ofSetVerticalSync(true);
	// ofBackgroundHex(0xa4c1b8);
    ofBackgroundHex(0xffffff);
	ofSetLogLevel(OF_LOG_NOTICE);
	
    
    // --- PHYSICS --- //
    setupPhysics();
    
    // ---- OPEN CV ---- //
    setupCV();

    // --- SETTINGS --- //
    scaleImage = 0.25f;
    nDivisions = 5;
    colChoice = 0;
    screenDiv = int(float(ofGetWidth())/float(nDivisions));
    
    for(int i=0; i<nDivisions; i++){
        insertNewImage.push_back(true);
    }
    maxImgReached = false;
}

//--------------------------------------------------------------
void testApp::update() {

    // --- PHYSICS --- //
    //updatePhysics();
    
    // ---- OPEN CV ---- //
    // updateCV();
    
    update2();
}

void testApp::update2(){
    for(int i=0; i<nDivisions; i++){
        if(insertNewImage[i] && !maxImgReached){
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
            colImgs[i].push_back(ofImage());
            colImgs[i].back().load(path);
            imgPos[i].push_back(ofPoint());
            imgPos[i].back().x = i * screenDiv;
            imgPos[i].back().y = 0;
            insertNewImage[i] = false;
        }
        for(int j=0; j<colImgs[i].size(); j++){
            imgPos[i][j].y += 3;
            if ( imgPos[i][j].y > ofGetHeight() ){
                imgPos[i][j].y = -50;
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
                colImgs[i][j].load(path);
            }
        }
    }
}

void testApp::draw2(){
    for (int i=0; i<nDivisions; i++){
        for (int j=0; j<colImgs[i].size(); j++){
            colImgs[i][j].draw(imgPos[i][j].x, imgPos[i][j].y,
                               colImgs[i][j].getWidth() *  scaleImage,
                               colImgs[i][j].getHeight() * scaleImage);
        }
    }
}

void testApp::resizeAll(){
    
}

//--------------------------------------------------------------
void testApp::draw() {

    // --- Symbols --- //
    //drawSymbols();
    
    // ---- OPEN CV ---- //
    //drawCV();
    
    draw2();
}

void testApp::setupPhysics(){
    box2d.init();
    box2d.setGravity(0, 0);
    //box2d.createBounds();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
}

void testApp::updatePhysics(){
    //ofRemove(circles, removeShapeOffScreen);
    
//    for(int i=0; i<circles.size(); i++){
//        if (!ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(circles[i].get()->getPosition())) {
//            //mappedImgs.erase(circles[i]);
//            //auto position = std::find(circles.begin(), circles.end(), circles[i]);
//            //circles.erase(position);
//        }
//    }
    
    box2d.update();
    
    if(geneCounter < 40){
        geneCounter++;
    } else {
        generateSymbol();
        geneCounter = 0;
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
    if (colChoice < nDivisions - 1){
        colChoice++;
    } else if (colChoice == nDivisions - 1){
        colChoice = 0;
    }
    screenPos = colChoice * screenDiv;
    
    float r = ofRandom(20, 30);
    circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
    circles.back().get()->setPhysics(3.0, 0.53, 0.1);
    ofVec2f attractionPt(screenPos, ofGetHeight() + 200);
    float x = ofRandom(0, ofGetWidth());
    float y = ofRandom(0, 0);
    circles.back().get()->setup(box2d.getWorld(), screenPos, 0.f, r);
    circles.back().get()->addAttractionPoint(attractionPt, 9);
    
    mappedImgs[circles.back()] = make_shared<ofImage>();
    mappedImgs[circles.back()]->load(path);
    //images.push_back(ofImage());
    //images[images.size()-1].load(path);
}

void testApp::drawSymbols(){
    for(int i=0; i<circles.size(); i++) {
        ofFill();
        ofSetHexColor(0xf6c738);
        float posX = circles[i].get()->getPosition().x;
        float posY = circles[i].get()->getPosition().y;
        ofPushMatrix();
        ofTranslate(posX + mappedImgs[circles[i]]->getWidth()/2*scaleImage + 10, posY);
        mappedImgs[circles[i]]->setAnchorPoint(0.5, 0.5);
        ofSetColor(255);
        mappedImgs[circles[i]]->draw(0, 0,
                                     mappedImgs[circles[i]]->getWidth()*scaleImage,
                                     mappedImgs[circles[i]]->getHeight()*scaleImage);
//
//        ofTranslate(posX + images[i].getWidth()/2*scaleImage + 10, posY);
//        images[i].setAnchorPercent(0.5, 0.5);
//        ofSetColor(255);
//        images[i].draw(0, 0, images[i].getWidth()*scaleImage, images[i].getHeight()*scaleImage);
        ofPopMatrix();
    }
    
    string info = "";
    info += "Press [c] for circles\n";
    info += "Press [b] for blocks\n";
    info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
    info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
    info += "Total Images: "+ofToString(mappedImgs.size())+"\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
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


// ----------------- //
// ---- OPEN CV ---- //
void testApp::setupCV(){
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

