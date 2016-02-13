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
    CalcCursorValue();
}

void ofxOscilloscope::draw()
{
    ofPushStyle();
    ofSetColor(backgroundColor);
    ofDrawRectangle(signalRectangle);
    if (bDrawGrid)
    {
        DrawGrid();
    }
    DrawRange();
    DrawLabels();
    for (auto signal:signals)
    {
        ofSetColor(colors[signal.first]);
        graphs[signal.first].draw();
    }
    
    ofPoint cursorPosition = ofPoint(ofGetMouseX(), ofGetMouseY());
    for (auto cursorValue:cursorValues)
    {
        ofSetColor(colors[cursorValue.first]);
        ofDrawBitmapString(ofToString(cursorValue.second), cursorPosition += ofPoint(0, 25));
    }
    
    
    
    ofDrawLine(cursorPosition.x, signalRectangle.getMinY(), cursorPosition.x, signalRectangle.getMaxY());
    
    ofNoFill();
    ofSetColor(gridColor);
    ofDrawRectangle(signalRectangle);
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
    float localMaximum = 0;
    float localMinimum = 0;
    
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

void ofxOscilloscope::DrawGrid()
{
    ofPoint leftTop = signalRectangle.getTopLeft();
    ofPoint rightBottom = signalRectangle.getBottomRight();
    
    ofPushStyle();
    ofSetColor(gridColor);
    for (float x = leftTop.x; x < rightBottom.x; x+= gridInterval)
    {
        ofDrawLine(ofPoint(x, leftTop.y), ofPoint(x, rightBottom.y));
    }
    for (float y = leftTop.y; y < rightBottom.y; y+= gridInterval)
    {
        ofDrawLine(ofPoint(leftTop.x, y), ofPoint(rightBottom.x, y));
    }
    ofPopStyle();
}

void ofxOscilloscope::DrawRange()
{
    ofDrawBitmapStringHighlight("Max : " + ofToString(rangeMax), signalRectangle.getTopLeft() + ofPoint(10, 20));
    ofDrawBitmapStringHighlight("Min : " + ofToString(rangeMin), signalRectangle.getBottomLeft() + ofPoint(10, -5));
}

void ofxOscilloscope::DrawLabels()
{
    ofPushStyle();
    ofPoint pos = signalRectangle.getBottomLeft() + ofPoint(0, -5);
    for (auto signal:signals)
    {
        ofSetColor(colors[signal.first]);
        ofDrawBitmapString(signal.first, pos += ofPoint(100, 0));
    }
    ofPopStyle();
}

void ofxOscilloscope::CalcCursorValue()
{
    ofPoint cursorPosition = ofPoint(ofGetMouseX(), ofGetMouseY());
    int cursorPointingIndex = (signalRectangle.getMaxX() - cursorPosition.x) / (signalRectangle.getWidth() / windowSize);
    if (signalRectangle.inside(cursorPosition))
    {
        for(auto signal:signals)
        {
            if (signal.second->size() > cursorPointingIndex)
            {
                cursorValues[signal.first] = signal.second->at(cursorPointingIndex);
            }
        }
    }
    
}























