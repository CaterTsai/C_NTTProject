#pragma once

#include "constParameter.h"
#include "wordUnit.h"
#include "urgMgr.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	float _timer;
	ofEasyCam _cam;

public:
	void addWords();
private:
	wordUnit _wordUnit;
	vector<ofVec3f> _wordPos;
	vector<wordUnit> _wordList;

#pragma region urgMgr
private:
	urgMgr _urgMgr;
#pragma endregion


};
