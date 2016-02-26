#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(ofColor::black);
    ofSetFrameRate(30);
    bPaused = false;
    myScope = new ofxOscilloscope(ofRectangle(0, 0, ofGetWidth(),(ofGetHeight() / 2)));
    myScope->assignSignals("sine signal", &sineSignal, ofColor::red);
    myScope->assignSignals("cosine signal", &cosineSignal, ofColor::green);
    
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    gui->addToggle("Pause");
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->addHeader(":: Drag Me To Reposiiton ::");
    gui->addFooter();
}

//--------------------------------------------------------------
void ofApp::update(){
    static float i = 0;
    if (!bPaused)
    {
        sineSignal.push_back(sin(i+=0.05));
        cosineSignal.push_back(cos(i));
    }
    myScope->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    myScope->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofColor color = ofColor::red;
    if ((key >= '0') && (key < '9'))
    {
        color = ofColor::green;
    }
    else if ((key >= 'a') && (key <= 'z'))
    {
        color = ofColor::yellow;
    }
    else if ((key >= 'A') && (key <= 'Z'))
    {
        color = ofColor::blue;
    }
    
    myScope->AddMarker(ofToString(key), color);
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (bPaused)
    {
        e.target->setLabel("Pause");
        bPaused = false;
    }
    else
    {
        e.target->setLabel("Play");
        bPaused = true;
    }
}

