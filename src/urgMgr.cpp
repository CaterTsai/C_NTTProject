
#include "urgWrapper.h"
#include "urgMgr.h"
#include "config.h"

#pragma region Trigger Area
ofEvent<string> urgMgr::triggerArea::_onTriggerOn = ofEvent<string>();
ofEvent<string> urgMgr::triggerArea::_onTriggerOff = ofEvent<string>();
//-----------------------------
urgMgr::triggerArea::triggerArea()
	:_id("")
	, _isTrigger(false)
	, _triggerTmp(false)
{}

//-----------------------------
urgMgr::triggerArea::triggerArea(ofRectangle area, string id)
	: _id(id)
	, _isTrigger(false)
	, _triggerTmp(false)
{
	_rect = area;
}

//-----------------------------
void urgMgr::triggerArea::draw(float ratio)
{
	auto drawRect = _rect;
	drawRect.scaleFromCenter(ratio);
	drawRect.x = drawRect.getCenter().x * ratio - drawRect.getWidth() * 0.5f;
	drawRect.y = drawRect.getCenter().y * ratio - drawRect.getHeight() * 0.5f;

	_isTrigger ? ofSetColor(255, 0, 0) : ofSetColor(0, 0, 255);
	ofDrawRectangle(drawRect);
}

//-----------------------------
void urgMgr::triggerArea::check(ofVec2f pos)
{
	if (_isTrigger)
	{
		return;
	}

	if (_rect.inside(pos))
	{
		_checkCounter++;
	}

	if (_checkCounter > config::getInstance()->_exUsgThreshold);
	{
		_isTrigger = true;
	}
}

//-----------------------------
void urgMgr::triggerArea::finish()
{
	if (_isTrigger)
	{
		if (_isTrigger != _triggerTmp)
		{
			ofNotifyEvent(urgMgr::triggerArea::_onTriggerOn, _id);
		}
	}
	else
	{
		if (_isTrigger != _triggerTmp)
		{
			ofNotifyEvent(urgMgr::triggerArea::_onTriggerOff, _id);
		}
	}
	_triggerTmp = _isTrigger;
}

//-----------------------------
void urgMgr::triggerArea::testCheck(ofRectangle testRect)
{
	if (_isTrigger)
	{
		return;
	}

	if (testRect.intersects(_rect))
	{
		_isTrigger = true;
	}
}

void urgMgr::triggerArea::updateArea(ofRectangle newRect)
{
	_rect = newRect;
}


//-----------------------------
void urgMgr::triggerArea::clear()
{
	
	_isTrigger = false;
	_checkCounter = false;
}
#pragma endregion


//-----------------------------
urgMgr::urgMgr()
	:_isSetup(false)
	, _startDeg(-90)
	, _endDeg(90)
	, _mm2pix(0.5f)
	, _testRectSize(50)
{
}

//-----------------------------
void urgMgr::setup(string ip, int port, float mm2pix)
{
	if (urgWrapper::getInstance()->conn(ip, port))
	{
		urgWrapper::getInstance()->enableSmooth(3);
		_isSetup = true;
	}
	else
	{
		_isSetup = false;
	}
	_mm2pix = mm2pix;
}

//-----------------------------
void urgMgr::update(float delta)
{
	if (!_isSetup)
	{
		return;
	}
	if (!urgWrapper::getInstance()->hasNewData())
	{
		return;
	}
	_distData = urgWrapper::getInstance()->getData();
	_degOfUnit = abs(_startDeg - _endDeg) / (float)urgWrapper::getInstance()->getDataSize();
	checkTrigger();
}

//-----------------------------
void urgMgr::draw(int x, int y)
{
	if (!_isSetup)
	{
		return;
	}

	ofPushMatrix();
	ofTranslate(x, y);
	{
		urgWrapper::getInstance()->draw(_mm2pix);
		drawArea();		
	}
	ofPopMatrix();
}

//-----------------------------
void urgMgr::start()
{
	if (_isSetup)
	{
		urgWrapper::getInstance()->start(_startDeg, _endDeg);
	}

}

//-----------------------------
void urgMgr::stop()
{
	if (_isSetup)
	{
		urgWrapper::getInstance()->stop();
	}

}

//-----------------------------
void urgMgr::addTriggerArea(string id, int x, int y, int width, int height)
{
	ofRectangle rect;
	rect.setFromCenter(x, y, width, height);
	triggerArea newArea(rect, id);
	_triggerAreaList.push_back(newArea);
}

//-----------------------------
void urgMgr::addListTriggerArea(int num, int dist, int width, int hegiht)
{
	float x = width * num * -0.5 + (0.5f * width);
	for (int i = 0; i < num; i++)
	{
		addTriggerArea(ofToString(i), x, dist, width, hegiht);
		x += width;
	}
}

//-----------------------------
void urgMgr::updateTriggerArea(int dist, int width, int height)
{
	float x = width * _triggerAreaList.size() * -0.5 + (0.5f * width);
	for (int i = 0; i < _triggerAreaList.size(); i++)
	{
		ofRectangle rect;
		rect.setFromCenter(x, dist, width, height);
		_triggerAreaList[i].updateArea(rect);
		x += width;
	}
}

//-----------------------------
void urgMgr::drawArea()
{
	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	for (auto& iter : _triggerAreaList)
	{
		iter.draw(_mm2pix);
	}
	ofPopStyle();
}

//-----------------------------
void urgMgr::checkTrigger()
{
	for (auto& iter : _triggerAreaList)
	{
		iter.clear();
	}

	for (int i = 0; i < urgWrapper::getInstance()->getDataSize(); i++)
	{
		ofVec2f pos = getPos(i, _distData[i]);
		for (auto& iter : _triggerAreaList)
		{
			iter.check(pos);
		}
	}

	for (auto& iter : _triggerAreaList)
	{
		iter.finish();
	}
}

//-----------------------------
ofVec2f urgMgr::getPos(int index, long dist)
{
	float rad = (_startDeg + index * _degOfUnit) * DEG_TO_RAD;
	return ofVec2f(sin(rad) * dist, cos(rad) * dist);
}

#pragma region Test Mode
//-----------------------------
void urgMgr::testUpdate(float delta)
{
	for (auto& iter : _triggerAreaList)
	{
		iter.clear();
	}
	for (int i = 0; i < _testPoint.size(); i++)
	{
		_testPoint[i].update(delta);
		_testRect[i].setPosition(_testPoint[i].getCurrentPosition());

		for (auto& tArea : _triggerAreaList)
		{
			tArea.testCheck(_testRect[i]);
		}
	}

	for (auto& tArea : _triggerAreaList)
	{
		tArea.finish();
	}
	
}

//-----------------------------
void urgMgr::testDraw(int x, int y)
{
	ofPushMatrix();
	ofTranslate(x, y);
	{
		for (auto iter : _testRect)
		{
			iter.scaleFromCenter(_mm2pix);
			iter.x = iter.getCenter().x * _mm2pix - iter.getWidth() * 0.5f;
			iter.y = iter.getCenter().y * _mm2pix - iter.getHeight() * 0.5f;
			ofDrawRectangle(iter);
		}
		drawArea();
	}
	ofPopMatrix();
}

//-----------------------------
void urgMgr::addTestPoint(ofVec2f s, ofVec2f e, float duration)
{
	ofRectangle rect;
	rect.set(s, _testRectSize, _testRectSize);
	_testRect.push_back(rect);

	ofxAnimatableOfPoint newAnim;
	newAnim.setDuration(duration);
	newAnim.setRepeatType(AnimRepeat::LOOP);
	newAnim.setPosition(s);
	_testPoint.push_back(newAnim);	
	_testPoint.back().animateTo(e);	
}

//-----------------------------
void urgMgr::clearTestPoint()
{
	_testPoint.clear();
	_testRect.clear();
}


#pragma endregion



