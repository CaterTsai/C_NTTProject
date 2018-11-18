#include "background.h"

void background::init()
{
	for (int i = 0; i < cBGSphereNum; i++)
	{
		ofVec3f v(ofRandom(1000, 2000), 0, 0);
		v.rotate(ofRandom(0, 360), ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)));
		_sphereList.push_back(v);
	}
	_rD = 0.0f;
	_rV = 3.0f;
	_animRotate.reset(0);
}

void background::update(float delta)
{
	_animRotate.update(delta);
	_rD += _rV * delta;
	if (_rD > 360)
	{
		_rD -= 360;
	}

}

void background::draw()
{
	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5);
	ofRotateX(-_animRotate.getCurrentValue());
	ofRotateY(_rD);
	ofPushStyle();
	ofNoFill();
	ofSetColor(255, 3);
	for (int i = 0; i < _sphereList.size(); i++)
	{
		ofDrawSphere(_sphereList[i], 10);
	}
	ofPopStyle();
	ofPopMatrix();
}

void background::rotate()
{
	float d = _animRotate.getCurrentValue();
	_animRotate.setDuration(cWordPosAnimT);
	_animRotate.animateTo(d + 90);
}
