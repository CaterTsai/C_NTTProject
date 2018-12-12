#pragma once

#include "constParameter.h"
#include "ofxXmlSettings.h"
class projectionCanvas {
public:
	projectionCanvas():
		_isInit(false)
		, _isBegin(false)
		, _isDebugMode(false)
		, _rowNum(0)
		, _colNum(0)
		, _ctrlIdx(0)
		, _canvasId(0)
	{}
	void init(int id, int x, int y, int width, int height, int rNum, int cNum);
	void draw();

	void begin();
	void end();

private:
	void load();
	void save();

private:
	int _canvasId;
	bool _isInit, _isBegin;
	
	int _rowNum, _colNum;
	ofFbo _canvas;
	ofVboMesh _mesh;

public:
	void debugUpdate();
	void debugDraw();
	void setDebugMode(bool val);
	bool getDebugMode();

	void debugCtrl(int key);
	void debugMouseCtrl(int x, int y);

private:
	int _ctrlIdx;
	bool _isDebugMode;


};