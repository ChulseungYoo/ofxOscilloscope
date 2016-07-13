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
	DrawControl();
	int colorIndex = 0;
	for (auto signal : (*signals))
	{
		string label = signal.first;
		if (signalToggles[label])
		{
			ofSetColor(colors[colorIndex++]);
			graphs[label].draw();
		}
	}
     
	ofPoint cursorPosition = ofPoint(ofGetMouseX(), ofGetMouseY());
	if (signalRectangle.inside(cursorPosition))
	{
		int colorIndex = 0;
		for (auto cursorValue : cursorValues)
		{
			if (signalToggles[cursorValue.first])
			{
				ofSetColor(colors[colorIndex++]);
				ofDrawBitmapString(ofToString(cursorValue.second), cursorPosition += ofPoint(0, -15));
			}
		}
		ofSetColor(ofColor::white);
		ofDrawLine(cursorPosition.x, signalRectangle.getMinY(), cursorPosition.x, signalRectangle.getMaxY());
	}
    DrawMarkers();
    ofNoFill();
    ofSetColor(gridColor);
    ofDrawRectangle(signalRectangle);
    ofPopStyle();
}

void ofxOscilloscope::assignSignals(map<string, tSignalInfo> *signals)
{
	this->signals = signals;
	for (auto signal : (*signals))
	{
		signalToggles.emplace(signal.first, false);
		controlPanel.add(signalToggles[signal.first].set(signal.first, false));
	}
	controlPanel.loadFromFile(title + "_setting.xml");
}

void ofxOscilloscope::AutoScale()
{
#if 0
    float localMaximum = 0;
    float localMinimum = 0;
	bool islocalInitialValueSet = false;
	for (auto signal : (*signals))
	{
		string label = signal.first;
		if (true == signalToggles[label])
		{
			int count = 0;
			for (int index = signal.second->size() - 1; ((index >= 0) && (count <= windowSize)); index--)
			{
				count++;
				if (!islocalInitialValueSet)
				{
					localMaximum = signal.second->at(index);
					localMinimum = localMaximum;
					islocalInitialValueSet = true;
				}
				else
				{
					if (localMaximum < signal.second->at(index))
					{
						localMaximum = signal.second->at(index);
					}
					else if (localMinimum > signal.second->at(index))
					{
						localMinimum = signal.second->at(index);
					}
				}
			}
		}
	}
#endif
}

void ofxOscilloscope::CalcGraph()
{
	for (auto signal:(*signals))
	{
		int count = 0;
		string label = signal.first;
		tSignalInfo sig = signal.second;
		graphs[label].clear();
		for (vector<float>::reverse_iterator it = sig.signal->rbegin(); it != sig.signal->rend(); ++it)
		{
			if (count++ < windowSize)
			{
				float x = ofMap(count, 0, windowSize, signalRectangle.getMaxX(), signalRectangle.getMinX());
				float y = ofMap((*it), sig.min, sig.max, signalRectangle.getMaxY(), signalRectangle.getMinY());
				graphs[label].addVertex(x, y);
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
//    ofDrawBitmapStringHighlight("Max : " + ofToString(rangeMax), signalRectangle.getTopLeft() + ofPoint(10, 20));
//    ofDrawBitmapStringHighlight("Min : " + ofToString(rangeMin), signalRectangle.getBottomLeft() + ofPoint(10, -5));
}

void ofxOscilloscope::DrawLabels()
{
    ofPushStyle();
    ofPoint pos = signalRectangle.getBottomLeft() + ofPoint(0, -5);
		int colorIndex = 0;
	for (auto signal : (*signals))
	{
		string label = signal.first;
		if (signalToggles[label])
		{
			if (colorIndex < colors.size())
			{
				ofSetColor(colors[colorIndex++]);
			}
			else
			{
				ofSetColor(ofColor::purple);
			}
			ofDrawBitmapString(label, pos += ofPoint(100, 0));
		}
	}
    ofPopStyle();
}

void ofxOscilloscope::CalcCursorValue()
{
    ofPoint cursorPosition = ofPoint(ofGetMouseX(), ofGetMouseY());
    int cursorPointingIndex = (signalRectangle.getMaxX() - cursorPosition.x) / (signalRectangle.getWidth() / windowSize);
    if (signalRectangle.inside(cursorPosition))
    {
		for (auto signal : (*signals))
		{
			string label = signal.first;
			if (signalToggles[label])
			{
				if (signal.second.signal->size() > cursorPointingIndex)
				{
					cursorValues[label] = signal.second.signal->at(signal.second.signal->size() - 1 - cursorPointingIndex);
				}
			}
		}
	}
    
}

void ofxOscilloscope::AddMarker(string label, ofColor color)
{
    tMarker aMarker;
    aMarker.color = color;
    aMarker.label = label;
    int index = 0;
    if (!signals->empty())
    {
#if 0
		markers.emplace(signals.begin()->second.begin()->second->size(), aMarker);
#endif
    }
}

void ofxOscilloscope::DrawMarkers()
{
#if 0
    ofPushStyle();
    if (!markers.empty())
    {
        for (map<int, tMarker>::reverse_iterator rit = markers.rbegin(); ((rit != markers.rend()) && (rit->first > (signals.begin()->second.begin()->second->size() - windowSize))); ++rit)
        {
            ofSetColor(rit->second.color);
            float xPosition = ofMap(rit->first, signals.begin()->second.begin()->second->size(), signals.begin()->second.begin()->second->size() - windowSize, signalRectangle.getMaxX(), signalRectangle.getMinX());
            ofDrawLine(ofPoint(xPosition, signalRectangle.getMinY()), ofPoint(xPosition, signalRectangle.getMaxY()));
            ofDrawBitmapStringHighlight(rit->second.label, ofPoint(xPosition, signalRectangle.getMinY() + 20));
        }
    }
    ofPopStyle();
#endif
}

void ofxOscilloscope::calculateRects(ofRectangle rect)
{
	entireRect = rect;
	if (rect.getWidth() < 400)
	{
		ofLog(OF_LOG_WARNING, "Oscilloscope Width is small");
	}
	if (rect.getHeight() < 200)
	{
		ofLog(OF_LOG_WARNING, "Oscilloscope Height is small");
	}
	controlRectangle.set(rect.getTopLeft(), 200, rect.getHeight());
	signalRectangle.set(controlRectangle.getTopRight(), rect.getBottomRight());
	controlPanel.setup(title, title + "_setting.xml", controlRectangle.getLeft(), controlRectangle.getTop());
	controlPanel.add(windowSize.set("WindowSize", 200, 100, 500));
}

void ofxOscilloscope::DrawControl()
{
	controlPanel.draw();
}

void ofxOscilloscope::DrawGraphs()
{

}
