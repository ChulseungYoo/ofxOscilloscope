//
//  ofxOscilloscope.h
//  example1
//
//  Created by Chulseung Yoo on 1/29/16.
//
//

#ifndef ofxOscilloscope_h
#define ofxOscilloscope_h

#include <stdio.h>
#include "ofMain.h"

class ofxOscilloscope{
public:
    ofxOscilloscope(ofRectangle signalRect)
    : bDrawGrid(true), bAutoScale(true), rangeMin(-10), rangeMax(10), windowSize(100), offset(0), signalRectangle(signalRect), backgroundColor(ofColor::black)
    {
        
    }
    void update();
    void draw();
    
    void assignSignals(string label, vector<float>* signal, ofColor color);
    
    
private:
    map<string, vector<float>*> signals;
    map<string, ofColor> colors;
    map<string, ofPolyline> graphs;
    ofColor backgroundColor;
    ofRectangle signalRectangle;
    
    bool bDrawGrid;
    bool bAutoScale;
    float rangeMin;
    float rangeMax;
    int windowSize;
    int offset;
    
    void AutoScale();
    void CalcGraph();
    void valueToRect();
    void drawGrid();
};

#endif /* ofxOscilloscope_h */
