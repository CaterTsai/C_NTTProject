#pragma once

#include "constParameter.h"
#include "wordUnit.h"
#include "urgMgr.h"
#include "maxSender.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	float _timer;
	ofEasyCam _cam;

#pragma region word unit
public:
	void addWords();
	eTextState triggerWord(int group, int id);
private:
	wordUnit _wordUnit;
	vector<ofVec3f> _wordPos;
	vector<wordUnit> _wordList;
#pragma endregion

#pragma region max Sender
public:
	void initMaxSender();
private:
	vector<maxSender> _maxSenderList;
#pragma endregion


#pragma region urgMgr
public:
	void initUrgMgr();
	void addTriggerAreas();
	void onTriggerOn(string& id);
	void onTriggerOff(string& id);
		
private:
	urgMgr _urgMgr;
#pragma endregion

//DEBUG
public:
	void triggerTestStart(float t);
	void triggerTestStop();
	void triggerTestUpdate(float delta);
	void triggerTestDraw();
private:
	bool _testStart;
	int _testIdx;
	float _testTimer, _testTimeSet;


};
