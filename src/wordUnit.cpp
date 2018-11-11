#include "wordUnit.h"
#include "text2Model.h"

#pragma region wordUnit

//------------------------------------
void wordUnit::init()
{
	initSphere();
}

//------------------------------------
void wordUnit::addText(wstring word)
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

	for (int i = 0; i < _textNum; i++)
	{
		_textAnimMgr[i].set(_textMgr[idx][i]);
	}

	_isSet = true;
}

//------------------------------------
void wordUnit::update(float delta)
{
	if (!_isSet)
	{
		return;
	}

	for (auto& iter : _textAnimMgr)
	{
		iter.update(delta);
	}
	updateSphere(delta);
}

//------------------------------------
void wordUnit::draw(int x, int y, int z)
{
	if (!_isSet)
	{
		return;
	}
	ofPushStyle();
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(x, y, z);

	for (auto& iter : _textAnimMgr)
	{
		iter.draw();
	}

	ofSetColor(255, _animAlpha.getCurrentValue());
	_sphere.drawWireframe();

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


	ofPopMatrix();
	ofPopStyle();
}

//------------------------------------
int wordUnit::getTextNum()
{
	return _textNum;
}

//------------------------------------
void wordUnit::triggerText(int idx)
{
	if (idx >= 0 && idx < getTextNum())
	{
		_textAnimMgr[idx].trigger();
	}

	triggerSphere();
}

//------------------------------------
void wordUnit::initSphere()
{
	_animSize.setDuration(cTextSphereAnimT);
	_animSize.setCurve(AnimCurve::EASE_IN_OUT_BACK);
	_animSize.reset(cTextFontSize * 2.5f);
	_animAlpha.setDuration(cTextSphereAnimT);
	_animAlpha.setCurve(AnimCurve::EASE_IN_OUT_BACK);
	_animAlpha.reset(70);

	_sphere.set(_animSize.getCurrentValue(), 20);
	_eState = eTextState::eTextCode;
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
	for (auto& iter : _textAnimMgr)
	{
		canTrigger &= (iter._eState != _eState);
	}

	if (canTrigger)
	{
		switch (_eState)
		{
		case eTextCode:
		{
			_eState = eTextLightOn;
			_animSize.animateTo(cTextFontSize * 2.0f);
			_animAlpha.animateTo(60);
			break;
		}
		case eTextLightOn:
		{
			_eState = eTextDisplayPart;
			_animSize.animateTo(cTextFontSize * 1.5f);
			_animAlpha.animateTo(20);
			break;
		}
		case eTextDisplayPart:
		{
			_eState = eTextDisplayAll;
			_animSize.animateTo(0);
			_animAlpha.animateTo(0);
			break;
		}
		case eTextDisplayAll:
		{
			_eState = eTextCode;
			_animSize.animateTo(cTextFontSize * 2.5f);
			_animAlpha.animateTo(70);
			break;
		}
		}
	}
}


#pragma endregion
