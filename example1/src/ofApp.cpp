#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(ofColor::black);
    ofSetFrameRate(30);
    myScope = new ofxOscilloscope(ofRectangle(0, 0, ofGetWidth(),ofGetHeight()));
    myScope->assignSignals("sine signal", &sineSignal, ofColor::red);
    myScope->assignSignals("cosine signal", &cosineSignal, ofColor::green);
}

//--------------------------------------------------------------
void ofApp::update(){
    static float i = 0;
    sineSignal.insert(sineSignal.begin(), sin(i+=0.1));
    cosineSignal.insert(cosineSignal.begin(), cos(i));
    myScope->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    myScope->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
