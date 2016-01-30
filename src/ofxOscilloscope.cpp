//
//  ofxOscilloscope.cpp
//  example1
//
//  Created by Chulseung Yoo on 1/29/16.
//
//

#include "ofxOscilloscope.h"

void ofxOscilloscope::update()
{
    if(bAutoScale)
    {
        AutoScale();
    }
    CalcGraph();
    
    
}

void ofxOscilloscope::draw()
{
    ofPushStyle();
    ofSetColor(backgroundColor);
    ofDrawRectangle(signalRectangle);
    for (auto signal:signals)
    {
        ofSetColor(colors[signal.first]);
        graphs[signal.first].draw();
    }
    ofPopStyle();
}

void ofxOscilloscope::assignSignals(string label, vector<float> *signal, ofColor color)
{
    signals.emplace(label, signal);
    colors.emplace(label, color);
    graphs.emplace(label, ofPolyline());
}

void ofxOscilloscope::AutoScale()
{
    
}

void ofxOscilloscope::CalcGraph()
{
    for (auto signal:signals)
    {
        int count = 0;
        graphs[signal.first].clear();
        for (vector<float>::iterator it = signal.second->begin(); it != signal.second->end(); ++it)
        {
            if (count++ < windowSize)
            {
                float x = ofMap(count, 0, windowSize, signalRectangle.getMaxX(), signalRectangle.getMinX());
                float y = ofMap((*it), rangeMin, rangeMax, signalRectangle.getMaxY(), signalRectangle.getMinY());
                graphs[signal.first].addVertex(x, y);
            }
            else
            {
                break;
            }
        }
    }
}



























