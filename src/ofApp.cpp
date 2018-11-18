#include "ofApp.h"
#include "text2Model.h"
#include "urgWrapper.h"

//--------------------------------------------------------------
void ofApp::setup(){

	bloomFilter::GetInstance()->init(ofGetWindowWidth(), ofGetWindowHeight(), true);
	bloomFilter::GetInstance()->filterEnable();
	text2Model::getInstance()->load("fontUC.ttc");

	_space.init();
	initWords();
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
	updateWordsPos(delta);
	_space.update(delta);
	//_urgMgr.update(delta);
	_urgMgr.testUpdate(delta);

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofPushMatrix();
	ofTranslate(0, _animPos.getCurrentValue());
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
	ofPopMatrix();
	_space.draw();

	_urgMgr.draw(ofGetWindowWidth() * 0.5f, 0);
	_urgMgr.testDraw(ofGetWindowWidth() * 0.5f, 0);
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 0, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'q':
	{
		_urgMgr.addTestPoint(ofVec2f(-1000, 250), ofVec2f(1000, 250), 10.0);
		break;
	}
	case 'w':
	{
		_urgMgr.addTestPoint(ofVec2f(1000, 250), ofVec2f(-1000, 250), ofRandom(10.0, 20.0));
		break;
	}
	case 'e':
	{
		_urgMgr.clearTestPoint();
		break;
	}
	case 'a':
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
	case 'z':
	{
		_space.rotate();
		break;
	}
	}
}

#pragma region WordUnit

//--------------------------------------------------------------
void ofApp::initWords()
{
	_wordList.resize(cTriggerGroupNum);
	ofxXmlSettings _xml;
	if (!_xml.load("textSet.xml"))
	{
		ofLog(OF_LOG_ERROR, "[ofApp::initWords]Load textSet failed");
		return;
	}
	_textSetNum = _xml.getNumTags("textSet");
	_displayId = 0;
	for (int i = 0; i < _textSetNum; i++)
	{
		_xml.pushTag("textSet", i);
		auto textNum = _xml.getNumTags("text");

		if (textNum != cTriggerGroupNum)
		{
			ofLog(OF_LOG_ERROR, "[ofApp::initWords]Text number is wrong");
			continue;
		}

		for (int j = 0; j < textNum; j++)
		{
			auto text = _xml.getValue("text", "", j);
			_wordList[j].addText(text);
		}

		_xml.popTag();
	}
	for (int i = 0; i < cTriggerGroupNum; i++)
	{
		_wordList[i].init(i);
		_wordList[i].setText(_displayId);
	}

	ofAddListener(wordUnit::_onWordDisplay, this, &ofApp::onWordDisplay);
	ofAddListener(wordUnit::_onWordOut, this, &ofApp::onWordOut);

	float unitW = ofGetWindowWidth() / 11.0f;
	float x = unitW;
	_wordPos.resize(cTriggerGroupNum);

	float noiseV = ofRandom(1.0);
	float delta = 0.01;
	for (int i = 0; i < cTriggerGroupNum; i++)
	{
		int y = ofGetWindowHeight() / 2.0f;
		y += (0.5 - ofNoise(i, noiseV)) * 400;
		_wordPos[i].set(x, y);
		x += unitW;
		noiseV += delta;
	}
	wordEnter();
}

//--------------------------------------------------------------
void ofApp::updateWordsPos(float delta)
{
	if (!_wordEnter)
	{
		return;
	}
	_animPos.update(delta);
	if (_animPos.hasFinishedAnimating() && _animPos.getPercentDone())
	{
		setTriggerEvent(true);
		_wordEnter = false;
	}
}

//--------------------------------------------------------------
void ofApp::wordEnter()
{
	_wordEnter = true;
	_animPos.setDuration(cWordPosAnimT);
	_animPos.reset(-ofGetWindowHeight());
	_animPos.animateTo(0.0f);

	_space.rotate();
}

//--------------------------------------------------------------
bool ofApp::checkWordAllDisplay()
{
	bool isAllFinish = true;
	for (auto& iter : _wordList)
	{
		isAllFinish &= iter.isAllDisplay();
	}

	return isAllFinish;

}

//--------------------------------------------------------------
bool ofApp::checkWordOut()
{
	bool isAllOut = true;
	for (auto& iter : _wordList)
	{
		isAllOut &= iter.isAllOut();
	}

	return isAllOut;
}


//--------------------------------------------------------------
eTextState ofApp::triggerWord(int group, int id)
{
	if (group < 0 || group >= _wordList.size())
	{
		return eTextUnknow;
	}

	eTextState rState = eTextCode;
	switch (_wordList[group].getTextNum())
	{
	case 1:
	{
		rState = _wordList[group].triggerText(0);
		break;
	}
	case 2:
	{
		if (id == 0)
		{
			rState = _wordList[group].triggerText(0);
		}
		else if (id == 2)
		{
			rState = _wordList[group].triggerText(1);
		}
		break;
	}
	case 3:
	{
		if (id != 3)
		{
			rState = _wordList[group].triggerText(id);
		}
		break;
	}
	}
	return rState;
}

//--------------------------------------------------------------
void ofApp::onWordDisplay(int & id)
{
	ofLog(OF_LOG_NOTICE, "[ofApp::onWordDisplay]ID :" + ofToString(id));
	if (checkWordAllDisplay())
	{
		ofLog(OF_LOG_NOTICE, "[ofApp::onWordDisplay]All Word Finish");
		setTriggerEvent(false);

		for (auto& iter : _wordList)
		{
			iter.explode();
		}
	}
}

//--------------------------------------------------------------
void ofApp::onWordOut(int & id)
{
	ofLog(OF_LOG_NOTICE, "[ofApp::onWordOut]ID :" + ofToString(id));
	if (checkWordOut())
	{
		ofLog(OF_LOG_NOTICE, "[ofApp::onWordOut]All Word Out");

		_displayId = (_displayId + 1) % _textSetNum;
		for (auto& iter : _wordList)
		{
			iter.setText(_displayId);
		}
		wordEnter();
	}
}
#pragma endregion

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

#pragma region urgMgr
//--------------------------------------------------------------
void ofApp::initUrgMgr()
{
	_urgMgr.setup("192.168.0.10", 10940, 0.2f);
	addTriggerAreas();
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

	auto newState = triggerWord(group, index);
	if (newState != eTextCode)
	{
		if (newState == eTextLightOn)
		{
			_maxSenderList[val].trigger(0);
		}
		else if (newState == eTextDisplayPart)
		{
			_maxSenderList[val].trigger(1);
		}
		else if (newState == eTextDisplayPart)
		{
			_maxSenderList[val].trigger(2);
		}
	}
}

//--------------------------------------------------------------
void ofApp::onTriggerOff(string & id)
{
	auto val = ofToInt(id);
	_maxSenderList[val].off();
}

//--------------------------------------------------------------
void ofApp::setTriggerEvent(bool isListen)
{
	if (isListen)
	{
		ofAddListener(urgMgr::triggerArea::_onTriggerOn, this, &ofApp::onTriggerOn);
		ofAddListener(urgMgr::triggerArea::_onTriggerOff, this, &ofApp::onTriggerOff);
	}
	else
	{
		ofRemoveListener(urgMgr::triggerArea::_onTriggerOn, this, &ofApp::onTriggerOn);
		ofRemoveListener(urgMgr::triggerArea::_onTriggerOff, this, &ofApp::onTriggerOff);
	}
}
#pragma endregion
