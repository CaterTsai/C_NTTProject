#include "ofApp.h"
#include "text2Model.h"
#include "urgWrapper.h"

//--------------------------------------------------------------
void ofApp::setup(){

	bloomFilter::GetInstance()->init(ofGetWindowWidth(), ofGetWindowHeight(), true);
	bloomFilter::GetInstance()->filterEnable();
	text2Model::getInstance()->load("font.ttf", "fontUC.ttc");
	addWords();

	initUrgMgr();
	initMaxSender();
	
	ofBackground(0);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	for (int i = 0; i < cTriggerGroupNum; i++)
	{
		_wordList[i].update(delta);
	}
	
	_urgMgr.update(delta);

	//DEBUG
	triggerTestUpdate(delta);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	_urgMgr.draw(ofGetWindowWidth() * 0.5f, 0);
	
	bloomFilter::GetInstance()->_bloom.begin();
	for (int i = 0; i < cTriggerGroupNum; i++)
	{
		_wordList[i].drawGlow(_wordPos[i].x, _wordPos[i].y, 0);
	}
	bloomFilter::GetInstance()->_bloom.end();

	for (int i = 0; i < cTriggerGroupNum; i++)
	{
		_wordList[i].draw(_wordPos[i].x, _wordPos[i].y, 0);
	}


	triggerTestDraw();
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 0, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'q':
	{
		for (int i = 0; i < cTriggerGroupNum; i++)
		{
			for (int j = 0; j < _wordList[i].getTextNum(); j++)
			{
				_wordList[i].triggerText(j);
			}
		}
		break;
	}
	case 'w':
	{
		triggerTestStart();
		break;
	}
	case 'a':
	{	
		_urgMgr.start();
		break;
	}
	case 's':
	{
		_urgMgr.stop();
		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::addWords()
{
	_wordList.resize(10);
	_wordList[0].addText(L"豪宅");
	_wordList[1].addText(L"百貨");
	_wordList[2].addText(L"巴士站");
	_wordList[3].addText(L"捷運");
	_wordList[4].addText(L"人群");
	_wordList[5].addText(L"吳寶春");
	_wordList[6].addText(L"耳朵石");
	_wordList[7].addText(L"這裡");
	_wordList[8].addText(L"以前");
	_wordList[9].addText(L"都沒有");

	for (int i = 0; i < cTriggerGroupNum; i++)
	{
		_wordList[i].init();
		_wordList[i].setText(0);
	}

	float unitW = ofGetWindowWidth() / 11.0f;
	float x = unitW;
	_wordPos.resize(10);

	float noiseV = ofRandom(1.0);
	float delta = 0.01;
	for (int i = 0; i < 10; i++)
	{
		int y = ofGetWindowHeight() / 2.0f;
		y += (0.5 - ofNoise(i, noiseV)) * 400;
		_wordPos[i].set(x, y);
		x += unitW;
		noiseV += delta;
	}

}

//--------------------------------------------------------------
void ofApp::triggerWord(int group, int id)
{
	if (group < 0 || group >= _wordList.size())
	{
		return;
	}

	switch (_wordList[group].getTextNum())
	{
	case 2:
	{
		if (id == 0)
		{
			_wordList[group].triggerText(0);
		}
		else if(id == 2)
		{
			_wordList[group].triggerText(1);
		}
		break;
	}
	case 3:
	{
		if (id != 3)
		{
			_wordList[group].triggerText(id);
		}
		break;
	}
	}
}

#pragma region Max Sender
//--------------------------------------------------------------
void ofApp::initMaxSender()
{
	_maxSenderList.resize(cTriggerNum);
	string ip = "127.0.0.1";
	int port = 7400;
	for (int i = 0; i < cTriggerNum; i++)
	{
		_maxSenderList[i].init(ip, port + i);
	}
}
#pragma endregion

//--------------------------------------------------------------
void ofApp::initUrgMgr()
{
	_urgMgr.setup("192.168.0.10", 10940, 0.2f);
	addTriggerAreas();

	ofAddListener(urgMgr::triggerArea::_onTriggerOn, this, &ofApp::onTriggerOn);
	ofAddListener(urgMgr::triggerArea::_onTriggerOff, this, &ofApp::onTriggerOff);
}

//--------------------------------------------------------------
void ofApp::addTriggerAreas()
{
	float unitW = 50;
	float unitH = 200;
	float x = unitW * cTriggerNum * -0.5 + (0.5f * unitW);
	for (int i = 0; i < cTriggerNum; i++)
	{
		_urgMgr.addTriggerArea(ofToString(i), x, 200, unitW, unitH);
		x += unitW;
	}
	
}

//--------------------------------------------------------------
void ofApp::onTriggerOn(string & id)
{
	auto val = ofToInt(id);
	int group = floor(val / (float)cTriggerEachGroup);
	int index = val % cTriggerEachGroup;

	triggerWord(group, index);
}

//--------------------------------------------------------------
void ofApp::onTriggerOff(string & id)
{
	auto val = ofToInt(id);
	int group = floor(val / (float)cTriggerEachGroup);
	int index = val % cTriggerEachGroup;

	triggerWord(group, index);
}

#pragma region DEBUG
//--------------------------------------------------------------
void ofApp::triggerTestStart()
{
	_testStart = true;
	_testIdx = 0;
	_testTimer = cTriggerTestT;

	
}

//--------------------------------------------------------------
void ofApp::triggerTestUpdate(float delta)
{
	if (!_testStart)
	{
		return;
	}

	_testTimer -= delta;
	if (_testTimer <= 0.0f)
	{
		int offId = (_testIdx - 1) < 0 ? cTriggerNum - 1 : (_testIdx - 1);
		_maxSenderList[offId].setOff();
		int group = floor(_testIdx / (float)cTriggerEachGroup);
		int index = _testIdx % cTriggerEachGroup;
		triggerWord(group, index);

		_maxSenderList[_testIdx].setOn();

		_testIdx = (_testIdx + 1) % cTriggerNum;
		
		_testTimer = cTriggerTestT;
	}
}

//--------------------------------------------------------------
void ofApp::triggerTestDraw()
{
	if (!_testStart)
	{
		return;
	}

	ofDrawBitmapStringHighlight("Test Index :" + ofToString(_testIdx), 0, 50);
}
#pragma endregion


