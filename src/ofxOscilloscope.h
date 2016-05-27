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

typedef struct SignalInfo {
	float min;
	float max;
	vector<float>* signal;
}tSignalInfo;


class ofxOscilloscope{
public:
	ofxOscilloscope(string title, ofRectangle rect)
	: title(title),
	bShowCursorValue(true),
    bDrawGrid(true),
    bAutoScale(false),
    windowSize(200),
    offset(0),
    backgroundColor(ofColor::black),
    gridColor(ofColor::gray),
	signals(NULL),
    gridInterval(50)
    {
		calculateRects(rect);
		colors.push_back(ofColor::red);
		colors.push_back(ofColor::green);
		colors.push_back(ofColor::blue);
		colors.push_back(ofColor::yellow);
		colors.push_back(ofColor::cyan);
		colors.push_back(ofColor::magenta);
		colors.push_back(ofColor::white);
		colors.push_back(ofColor::purple);
		colors.push_back(ofColor::beige);
		colors.push_back(ofColor::gray);
		colors.push_back(ofColor::red);
    }
    void update();
    void draw();
	ofRectangle getRect() { return entireRect; }
	void assignSignals(map<string, SignalInfo>* signals);
    void AddMarker(string markerLabel, ofColor color);
    
private:
	string title;
	map<string, tSignalInfo> * signals;
	map<string, ofParameter<bool>> signalToggles;
    vector<ofColor> colors;
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
