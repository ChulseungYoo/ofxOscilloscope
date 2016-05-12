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
	for (auto rangeGroup : signals)
	{
		for (auto signal:rangeGroup.second)
		{
			if (signalToggles[signal.first])
			{
				ofSetColor(colors[signal.first]);
				graphs[signal.first].draw();
			}
		}
	}
     
	ofPoint cursorPosition = ofPoint(ofGetMouseX(), ofGetMouseY());
	if (signalRectangle.inside(cursorPosition))
	{
		for (auto cursorValue : cursorValues)
		{
			ofSetColor(colors[cursorValue.first]);
			ofDrawBitmapString(ofToString(cursorValue.second), cursorPosition += ofPoint(0, -15));
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

void ofxOscilloscope::assignSignals(string rangeGroup, string label, vector<float> *signal, ofColor color)
{
    signals[rangeGroup].emplace(label, signal);
    colors.emplace(label, color);
    graphs.emplace(label, ofPolyline());
	signalToggles.emplace(label, false);
	controlPanel.add(signalToggles[label].set(label, false));
	controlPanel.loadFromFile(title + "_setting.xml");
}

void ofxOscilloscope::AutoScale()
{
    float localMaximum = 0;
    float localMinimum = 0;
	bool islocalInitialValueSet = false;
	for (auto rangeGroup : signals)
	{
		for (auto signal : rangeGroup.second)
		{
			if (true == signalToggles[signal.first])
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
		ranges[rangeGroup.first].x = localMaximum;
		ranges[rangeGroup.first].y = localMinimum;

	}
    
    
}

void ofxOscilloscope::CalcGraph()
{
	for (auto rangeGroup : signals)
	{
		for (auto signal:rangeGroup.second)
		{
			int count = 0;
			graphs[signal.first].clear();
			for (vector<float>::reverse_iterator it = signal.second->rbegin(); it != signal.second->rend(); ++it)
			{
				if (count++ < windowSize)
				{
					float x = ofMap(count, 0, windowSize, signalRectangle.getMaxX(), signalRectangle.getMinX());
					float y = ofMap((*it), ranges[rangeGroup.first].y, ranges[rangeGroup.first].x, signalRectangle.getMaxY(), signalRectangle.getMinY());
					graphs[signal.first].addVertex(x, y);
				}
				else
				{
					break;
				}
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
    for (auto rangeGroup:signals)
	{
		for (auto signal : rangeGroup.second)
		{
			ofSetColor(colors[signal.first]);
			ofDrawBitmapString(signal.first, pos += ofPoint(100, 0));
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
		for (auto rangeGroup : signals)
		{
			for (auto signal : rangeGroup.second)
			{
				if (signalToggles[signal.first])
				{
					if (signal.second->size() > cursorPointingIndex)
					{
						cursorValues[signal.first] = signal.second->at(signal.second->size() - 1 - cursorPointingIndex);
					}
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
    if (!signals.empty())
    {
        markers.emplace(signals.begin()->second.begin()->second->size(), aMarker);
    }
}

void ofxOscilloscope::DrawMarkers()
{
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
}

void ofxOscilloscope::calculateRects(ofRectangle rect)
{
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
