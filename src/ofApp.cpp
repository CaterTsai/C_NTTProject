#include "ofApp.h"
#include "text2Model.h"
#include "urgWrapper.h"
#include "config.h"
//--------------------------------------------------------------
void ofApp::setup() {

	config::getInstance()->init();
	_canvas[0].init(0, 0, 0, config::getInstance()->_exWindowWidth * 0.5, config::getInstance()->_exWindowHeight, config::getInstance()->_exMeshColNum, config::getInstance()->_exMeshRowNum);
	_canvas[1].init(1, 0, 0, config::getInstance()->_exWindowWidth * 0.5, config::getInstance()->_exWindowHeight, config::getInstance()->_exMeshColNum, config::getInstance()->_exMeshRowNum);

	ofSetWindowShape(config::getInstance()->_exWindowWidth, config::getInstance()->_exWindowHeight);
	ofSetWindowPosition(0, 0);

	bloomFilter::GetInstance()->init(config::getInstance()->_exWindowWidth, config::getInstance()->_exWindowHeight, true);
	bloomFilter::GetInstance()->filterEnable();
	text2Model::getInstance()->load("fontUC.otf");

	_space.init(config::getInstance()->_exBGSphereNum);
	initWords();
	initUrgMgr();
	initMaxSender();

	ofBackground(0);
	_timer = ofGetElapsedTimef();

#ifdef _DEBUG
	_debugMode = true;
	setConfigListener(true);

#else
	_debugMode = false;
	ofHideCursor();
#endif

	_maxChangeSong.change(_displayId + 1);
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;
	updateWords(delta);

	_space.update(delta);

	if (config::getInstance()->_exEnableTestMode)
	{
		_urgMgr.testUpdate(delta);
	}
	else
	{
		_urgMgr.update(delta);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	_canvas[0].begin();
	ofPushMatrix();
	ofTranslate(0, _animPos.getCurrentValue());
	bloomFilter::GetInstance()->_bloom.begin();
	for (int i = 0; i < cTriggerGroupNum / 2.0f; i++)
	{
		_wordList[i].drawGlow(_wordPos[i].x, _wordPos[i].y, 0);
	}
	bloomFilter::GetInstance()->_bloom.end();

	for (int i = 0; i < cTriggerGroupNum / 2.0f; i++)
	{
		_wordList[i].draw(_wordPos[i].x, _wordPos[i].y, 0);
	}
	ofPopMatrix();
	_canvas[0].end();

	_canvas[1].begin();
	ofPushMatrix();
	ofTranslate(0, _animPos.getCurrentValue());
	bloomFilter::GetInstance()->_bloom.begin();
	for (int i = cTriggerGroupNum / 2.0f; i < cTriggerGroupNum; i++)
	{
		_wordList[i].drawGlow(_wordPos[i].x, _wordPos[i].y, 0);
	}
	bloomFilter::GetInstance()->_bloom.end();

	for (int i = cTriggerGroupNum / 2.0f; i < cTriggerGroupNum; i++)
	{
		_wordList[i].draw(_wordPos[i].x, _wordPos[i].y, 0);
	}
	ofPopMatrix();

	_canvas[1].end();


	_canvas[0].draw();
	
	ofPushMatrix();
	ofTranslate(config::getInstance()->_exWindowWidth * 0.5, 0);
	_canvas[1].draw();
	ofPopMatrix();
	
	_space.draw();

	if (_debugMode)
	{
		_canvas[0].debugUpdate();
		_canvas[1].debugUpdate();

		_canvas[0].debugDraw();

		ofPushMatrix();
		ofTranslate(config::getInstance()->_exWindowWidth * 0.5, 0);
		_canvas[1].debugDraw();
		_space.draw();
		ofPopMatrix();

		if (config::getInstance()->_exEnableTestMode)
		{
			_urgMgr.testDraw(ofGetWindowWidth() * 0.5f, 0);
		}
		else
		{
			_urgMgr.draw(ofGetWindowWidth() * 0.5f, 0);
		}
		config::getInstance()->draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == '0')
	{
		_debugMode ^= true;
		if (_debugMode)
		{
			ofShowCursor();
		}
		else
		{
			ofHideCursor();
		}
		setConfigListener(_debugMode);

	}
	if (_debugMode)
	{
		switch (key)
		{
		case 'q':
		{
			_urgMgr.addTestPoint(ofVec2f(-5000, 1000), ofVec2f(5000, 1000), 10.0);
			break;
		}
		case 'w':
		{
			_urgMgr.addTestPoint(ofVec2f(5000, 1000), ofVec2f(-5000, 1000), ofRandom(10.0, 20.0));
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
			_wordList[0].triggerText(0);
			break;
		}
		case 's':
		{
			config::getInstance()->save();
			break;
		}
		case 'd':
		{
			_canvas[0].setDebugMode(!_canvas[0].getDebugMode());
			break;
		}
		case 'f':
		{
			_canvas[1].setDebugMode(!_canvas[1].getDebugMode());
			break;
		}
		}

		if (_canvas[0].getDebugMode())
		{
			_canvas[0].debugCtrl(key);
		}
		if (_canvas[1].getDebugMode())
		{
			_canvas[1].debugCtrl(key);
		}
	}

}

void ofApp::mouseDragged(int x, int y, int button)
{
	if (_debugMode)
	{
		if (_canvas[0].getDebugMode() && button == OF_MOUSE_BUTTON_LEFT)
		{
			_canvas[0].debugMouseCtrl(x, y);
		}
		if (_canvas[1].getDebugMode() && button == OF_MOUSE_BUTTON_LEFT)
		{
			_canvas[1].debugMouseCtrl(x - config::getInstance()->_exWindowWidth * 0.5f, y);
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
	_isWaitExplode = false;
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

	float unitW = (config::getInstance()->_exWindowWidth * 0.5) / 6.0f;
	float x = unitW;
	_wordPos.resize(cTriggerGroupNum);
	float noiseV = ofRandom(1.0);
	float delta = 0.01;
	for (int i = 0; i < cTriggerGroupNum ; i++)
	{
		if (i == 5)
		{
			x = unitW;
		}
		int y = config::getInstance()->_exWindowHeight / 2.0f;
		y += (0.5 - ofNoise(i, noiseV)) * 400;
		_wordPos[i].set(x, y);
		x += unitW;
		noiseV += delta;
	}
	wordEnter();
}


//--------------------------------------------------------------
void ofApp::updateWords(float delta)
{
	for (int i = 0; i < cTriggerGroupNum; i++)
	{
		_wordList[i].update(delta);
	}
	updateWordsPos(delta);

	if (_isWaitExplode)
	{
		_explodeTimer -= delta;
		if (_explodeTimer <= 0.0f)
		{
			for (auto& iter : _wordList)
			{
				iter.explode();
			}
			_isWaitExplode = false;
		}
	}
}

//--------------------------------------------------------------
void ofApp::wordEnter()
{
	_wordEnter = true;
	_animPos.setDuration(config::getInstance()->_transitionT);
	_animPos.reset(-config::getInstance()->_exWindowHeight);
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
	case 7: {
		if (id == 0)
		{
			rState = _wordList[group].triggerText(0);
			rState = _wordList[group].triggerText(1);
			rState = _wordList[group].triggerText(2);
		}
		else if (id == 1)
		{
			rState = _wordList[group].triggerText(3);
		}
		else if (id == 2)
		{
			rState = _wordList[group].triggerText(4);
			rState = _wordList[group].triggerText(5);
			rState = _wordList[group].triggerText(6);
		}
	}
	}
	return rState;
}

//--------------------------------------------------------------
void ofApp::onWordDisplay(int & id)
{
	//ofLog(OF_LOG_NOTICE, "[ofApp::onWordDisplay]ID :" + ofToString(id));
	if (checkWordAllDisplay())
	{
		//ofLog(OF_LOG_NOTICE, "[ofApp::onWordDisplay]All Word Finish");
		setTriggerEvent(false);
		_isWaitExplode = true;
		_explodeTimer = config::getInstance()->_explodeWaitT;

	}
}

//--------------------------------------------------------------
void ofApp::onWordOut(int & id)
{
	//ofLog(OF_LOG_NOTICE, "[ofApp::onWordOut]ID :" + ofToString(id));
	if (checkWordOut())
	{
		//ofLog(OF_LOG_NOTICE, "[ofApp::onWordOut]All Word Out");

		_displayId = (_displayId + 1) % _textSetNum;
		for (auto& iter : _wordList)
		{
			iter.setText(_displayId);
		}
		_maxChangeSong.change(_displayId + 1);
		wordEnter();
	}
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
#pragma endregion

#pragma region Max Sender
//--------------------------------------------------------------
void ofApp::initMaxSender()
{
	_maxSenderList.resize(cTriggerNum);
	string ip = config::getInstance()->_exMaxIP;
	int port = config::getInstance()->_exMaxPort;
	for (int i = 0; i < cTriggerNum; i++)
	{
		_maxSenderList[i].init(ip, port + i);
	}

	_maxChangeSong.init(ip, 7440);
}
#pragma endregion

#pragma region urgMgr
//--------------------------------------------------------------
void ofApp::initUrgMgr()
{
	_urgMgr.setup(config::getInstance()->_exUrgIP, config::getInstance()->_exUrgPort, config::getInstance()->_exUrgMM2Pix);
	_urgMgr.addListTriggerArea(cTriggerNum, config::getInstance()->_triggerDist, config::getInstance()->_triggerW, config::getInstance()->_triggerH);
	//addTriggerAreas();
	_urgMgr.start();
}

//--------------------------------------------------------------
void ofApp::addTriggerAreas()
{
	float unitW = config::getInstance()->_triggerW;
	float unitH = config::getInstance()->_triggerH;
	float x = unitW * cTriggerNum * -0.5 + (0.5f * unitW);
	for (int i = 0; i < cTriggerNum; i++)
	{
		_urgMgr.addTriggerArea(ofToString(i), x, config::getInstance()->_triggerDist, unitW, unitH);
		x += unitW;
	}
}

//--------------------------------------------------------------
void ofApp::updateTriggerArea()
{
	_urgMgr.updateTriggerArea(config::getInstance()->_triggerDist, config::getInstance()->_triggerW, config::getInstance()->_triggerH);
}

//--------------------------------------------------------------
void ofApp::onTriggerOn(string & id)
{
	auto val = ofToInt(id);
	int group = floor(val / (float)cTriggerEachGroup);
	int index = val % cTriggerEachGroup;

	auto newState = triggerWord(group, index);

	if (newState == eTextLightOn || newState == eTextCode2LightOn)
	{
		_maxSenderList[val].trigger(0);
	}
	else if (newState == eTextDisplayPart || newState == eTextLightOn2DisplayPart)
	{
		_maxSenderList[val].trigger(1);
	}
	else if (newState == eTextDisplayAll || newState == eTextDisplayPart2All)
	{
		_maxSenderList[val].trigger(2);
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

#pragma region Config
//--------------------------------------------------------------
void ofApp::setConfigListener(bool isListen)
{
	if (isListen)
	{
		config::getInstance()->_updateTrigger.addListener(this, &ofApp::onTriggerAreaUpdate);
	}
	else
	{
		config::getInstance()->_updateTrigger.removeListener(this, &ofApp::onTriggerAreaUpdate);
	}

}
//--------------------------------------------------------------
void ofApp::onTriggerAreaUpdate()
{
	updateTriggerArea();
}
#pragma endregion
