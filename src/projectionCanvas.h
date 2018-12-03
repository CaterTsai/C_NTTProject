#pragma once

#include "constParameter.h"

class projectionCanvas {
public:
	projectionCanvas():
		_isInit(false)
		, _isBegin(false)
	{}
	void init(int x, int y, int width, int height, int rNum, int cNum);
	void draw();
	void drawMesh();
	
	void begin();
	void end();
private:
	bool _isInit, _isBegin;
	ofFbo _canvas;
	ofVboMesh _mesh;
};