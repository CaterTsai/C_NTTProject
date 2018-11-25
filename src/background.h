#pragma once

#include "constParameter.h"
#include "ofxAnimatableFloat.h"


class background
{
public:
	void init(int num);
	void update(float delta);
	void draw();

	void rotate();

private:
	ofxAnimatableFloat _animRotate;
	vector<ofPoint> _sphereList;
	float _rD, _rV;
};