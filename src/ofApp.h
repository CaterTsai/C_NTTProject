#pragma once

#include "ofxXmlSettings.h"
#include "constParameter.h"
#include "wordUnit.h"
#include "urgMgr.h"
#include "maxSender.h"
#include "background.h"

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
	void initWords();
	void updateWordsPos(float delta);
	void wordEnter();
	bool checkWordAllDisplay();
	bool checkWordOut();
	eTextState triggerWord(int group, int id);

	void onWordDisplay(int& id);
	void onWordOut(int& id);
private:
	bool _wordEnter;
	int _displayId, _textSetNum;	
	ofxAnimatableFloat _animPos;
	vector<ofVec3f> _wordPos;
	vector<wordUnit> _wordList;
#pragma endregion

private:
	background _space;

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

	void setTriggerEvent(bool isListen);
private:
	urgMgr _urgMgr;
#pragma endregion


};
