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
#include "ofxGui.h"

typedef struct marker{
    string label;
    ofColor color;
}tMarker;

class ofxOscilloscope{
public:
	ofxOscilloscope(string title, ofRectangle rect)
	: title(title),
	bShowCursorValue(true),
    bDrawGrid(true),
    bAutoScale(true),
    windowSize(200),
    offset(0),
    backgroundColor(ofColor::black),
    gridColor(ofColor::gray),
    gridInterval(50)
    {
		calculateRects(rect);
    }
    void update();
    void draw();
	ofRectangle getRect() { return entireRect; }
	void assignSignals(string rangeGroup, string label, vector<float>* signal, ofColor color);
    void AddMarker(string markerLabel, ofColor color);
    
private:
	string title;
    map<string/*range*/, map<string, vector<float>*>> signals;
	map<string, ofParameter<bool>> signalToggles;
    map<string, ofColor> colors;
    map<string, ofPolyline> graphs;
    map<string, float> cursorValues;
    map<int, tMarker> markers;           /* <index, label> */
    ofColor backgroundColor;
    ofColor gridColor;
	ofRectangle entireRect;
    ofRectangle signalRectangle;
	ofRectangle controlRectangle;
	ofRectangle titleRectangle;

	ofxPanel controlPanel;
    bool bShowCursorValue;
    bool bDrawGrid;
    bool bAutoScale;
	map<string, ofVec2f> ranges;
    ofParameter<int> windowSize;
    int offset;
    float gridInterval;
    
	void calculateRects(ofRectangle rect);
    void AutoScale();
    void CalcGraph();
    void CalcCursorValue();
    void DrawGrid();
    void DrawRange();
    void DrawLabels();
    void DrawMarkers();
	void DrawControl();
	void DrawGraphs();
};

#endif /* ofxOscilloscope_h */
