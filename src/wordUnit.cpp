#include "wordUnit.h"
#include "text2Model.h"
#include "config.h"

#pragma region wordUnit

//------------------------------------
void wordUnit::init(int id)
{
	_id = id;
	initSphere();
}

//------------------------------------
void wordUnit::addText(wstring word)
{
	addText(text2Model::getInstance()->ws2s(word));
}

//------------------------------------
void wordUnit::addText(string word)
{
	vector<text> textList;
	vector<ofVboMesh> meshList;
	vector<int> meshGroup;
	text2Model::getInstance()->translate(word, 3, meshList, meshGroup);
	textList.resize(meshGroup.size());

	int sIdx = 0;
	for (int i = 0; i < meshGroup.size(); i++)
	{
		text unit;
		unit.init(vector<ofVboMesh>(meshList.begin() + sIdx, meshList.begin() + meshGroup[i]));
		sIdx = meshGroup[i];
		textList[i] = unit;
	}
	_textMgr.push_back(textList);
}

//------------------------------------
void wordUnit::setText(int idx)
{
	if (idx < 0 || idx >= _textMgr.size())
	{
		ofLog(OF_LOG_ERROR, "[wordUnit::setText]Index out of range");
		return;
	}
	_textNum = _textMgr[idx].size();

	for (int i = 0; i < cTriggerEachGroup; i++)
	{
		if (i < _textNum)
		{
			_textAnimMgr[i].set(_textMgr[idx][i]);
		}
		else
		{
			_textAnimMgr[i].clear();
		}

	}

	initSphere();
	_animTextAlpha.reset(255);
	_isSet = true;

}

//------------------------------------
void wordUnit::update(float delta)
{
	if (!_isSet)
	{
		return;
	}

	_animTextAlpha.update(delta);
	for (auto& iter : _textAnimMgr)
	{
		iter.update(delta);
	}
	updateSphere(delta);

	if (_needCheckAnim)
	{
		checkAnim();
	}
}

//------------------------------------
void wordUnit::draw(int x, int y, int z)
{
	if (!_isSet)
	{
		return;
	}
	ofPushStyle();
	ofSetColor(255, _animTextAlpha.getCurrentValue());
	ofPushMatrix();
	ofTranslate(x, y, z);

	for (auto& iter : _textAnimMgr)
	{
		iter.draw();
	}

	//if (!_glowSphere)
	//{
	//	ofSetColor(255, _animAlpha.getCurrentValue());
	//	_sphere.drawWireframe();
	//}


	ofPopMatrix();
	ofPopStyle();
}

//------------------------------------
void wordUnit::drawGlow(int x, int y, int z)
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(x, y, z);

	for (auto& iter : _textAnimMgr)
	{
		iter.drawGlow();
	}

	//if (_glowSphere)
	//{
	//	ofSetColor(255, _animAlpha.getCurrentValue());
	//	_sphere.drawWireframe();
	//}

	ofPopMatrix();
	ofPopStyle();
}

//------------------------------------
bool wordUnit::isAllDisplay()
{
	bool result = false;
	if (_eState == eTextDisplayAll && _animAlpha.hasFinishedAnimating() && _animSize.getPercentDone() == 1.0f)
	{
		bool isAllFinish = true;
		for (int i = 0; i < _textNum; i++)
		{
			isAllFinish &= (_textAnimMgr[i]._eState == eTextState::eTextDisplayAll);
		}
		result = isAllFinish;
	}
	return result;
}

//------------------------------------
bool wordUnit::isAllOut()
{
	bool result = false;
	if (_eState == eTextExplode && _animAlpha.hasFinishedAnimating() && _animSize.getPercentDone() == 1.0f)
	{
		bool isAllOut = true;
		for (int i = 0; i < _textNum; i++)
		{
			isAllOut &= (_textAnimMgr[i]._eState == eTextState::eTextOut);
		}
		result = isAllOut;
	}
	return result;
}

//------------------------------------
int wordUnit::getTextNum()
{
	return _textNum;
}

//------------------------------------
eTextState wordUnit::triggerText(int idx)
{
	if (idx < 0 || idx >= getTextNum())
	{
		return eTextUnknow;
	}

	if (_textAnimMgr[idx]._eState != eTextDisplayAll)
	{
		_textAnimMgr[idx].trigger();
		triggerSphere();
	}
	return _textAnimMgr[idx]._eState;
}

//------------------------------------
void wordUnit::checkAnim()
{
	if (isAllDisplay())
	{
		_needCheckAnim = false;
		ofNotifyEvent(wordUnit::_onWordDisplay, _id);
	}
	else if (isAllOut())
	{
		_needCheckAnim = false;
		ofNotifyEvent(wordUnit::_onWordOut, _id);
	}
}

//------------------------------------
void wordUnit::explode()
{
	if (_eState != eTextDisplayAll)
	{
		return;
	}

	_eState = eTextExplode;
	_animTextAlpha.setDuration(cTextOutAnimT);
	_animTextAlpha.setCurve(AnimCurve::LATE_EASE_IN_EASE_OUT);
	_animSize.setDuration(cTextOutAnimT);
	_animAlpha.setDuration(cTextOutAnimT);
	_animAlpha.setCurve(AnimCurve::EASE_IN_EASE_OUT);

	_glowSphere = true;

	for (int i = 0; i < _textNum; i++)
	{
		_textAnimMgr[i].explode();
	}
	_animTextAlpha.animateTo(0.0f);
	_animSize.animateFromTo(0.0, cTextOutLevel * 0.25);
	_animAlpha.animateFromTo(255.0f, 0.0f);
	_needCheckAnim = true;

}


#pragma region Sphere
//------------------------------------
void wordUnit::initSphere()
{
	_animSize.setDuration(cTextSphereAnimT);
	_animSize.setCurve(AnimCurve::EASE_IN_OUT_BACK);
	_animSize.setEaseBackOffset(2.0);
	_animSize.reset(config::getInstance()->_exFontSize * 2.5f);
	_animAlpha.setDuration(cTextSphereAnimT);
	_animAlpha.setCurve(AnimCurve::EASE_IN_OUT_BACK);
	_animAlpha.setEaseBackOffset(2.0);
	_animAlpha.reset(70);

	_sphere.set(_animSize.getCurrentValue(), 20);
	_eState = eTextState::eTextCode;

	_glowSphere = false;
}

//------------------------------------
void wordUnit::updateSphere(float delta)
{
	_animSize.update(delta);
	_animAlpha.update(delta);

	_sphere.setRadius(_animSize.getCurrentValue());
}

//------------------------------------
void wordUnit::triggerSphere()
{
	bool canTrigger = true;

	for (int i = 0; i < _textNum; i++)
	{
		switch (_eState)
		{
		case eTextCode:
		{
			canTrigger &= (_textAnimMgr[i]._eState == eTextLightOn || _textAnimMgr[i]._eState == eTextCode2LightOn);
			break;
		}
		case eTextLightOn:
		{
			canTrigger &= (_textAnimMgr[i]._eState == eTextDisplayPart || _textAnimMgr[i]._eState == eTextLightOn2DisplayPart);
			break;
		}
		case eTextDisplayPart:
		{
			canTrigger &= (_textAnimMgr[i]._eState == eTextDisplayAll || _textAnimMgr[i]._eState == eTextDisplayPart2All);
			break;
		}
		}
	}

	if (canTrigger)
	{
		switch (_eState)
		{
		case eTextCode:
		{
			_eState = eTextLightOn;
			_animSize.animateTo(config::getInstance()->_exFontSize * 2.0f);
			_animAlpha.animateTo(60);
			break;
		}
		case eTextLightOn:
		{
			_eState = eTextDisplayPart;
			_animSize.animateTo(config::getInstance()->_exFontSize * 1.5f);
			_animAlpha.animateTo(20);
			break;
		}
		case eTextDisplayPart:
		{
			_eState = eTextDisplayAll;
			_animSize.animateTo(config::getInstance()->_exFontSize * 0.5f);
			_animAlpha.animateTo(0);
			_needCheckAnim = true;
			break;
		}
		case eTextDisplayAll:
		{
			break;
		}
		}

		for (int i = 0; i < _textNum; i++)
		{
			_textAnimMgr[i].setTrigger(true);
		}
	}
}
#pragma endregion

#pragma region Event
ofEvent<int> wordUnit::_onWordDisplay = ofEvent<int>();
ofEvent<int> wordUnit::_onWordOut = ofEvent<int>();
#pragma endregion


#pragma endregion
