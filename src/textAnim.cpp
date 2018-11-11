#include "textAnim.h"

#pragma region pUnit
//------------------------------------
textAnim::pUnit::pUnit()
	:_r(1.0f)
	, _theta(0)
	, _sigma(0)
	, _vT(0)
	, _vS(0)
	, _rD(0)
	, _vR(0)
	, _eState(eRotateMode)
{}

//------------------------------------
textAnim::pUnit::pUnit(float r, ofVec3f pos)
	:_r(r)
	, _eState(eRotateMode)
{
	_theta = ofRandom(0, PI);
	_sigma = ofRandom(0, TWO_PI);

	_vT = ofRandom(-TWO_PI, TWO_PI) * 0.1f;
	_vS = ofRandom(-TWO_PI, TWO_PI) * 0.1f;

	_axis.set(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
	_axis.normalize();

	_rD = ofRandom(0, 360);
	_vR = ofRandom(-360, 360) * 0.1f;

	_stayPos = pos;
	_animPos.setDuration(cTextAnimT);
	_animRotate.setDuration(cTextAnimT * 1.2);
}

//------------------------------------
void textAnim::pUnit::update(float delta)
{
	_animPos.update(delta);
	_animRotate.update(delta);
	switch (_eState)
	{
	case eRotateMode:
	{
		_theta += _vT * delta;
		_sigma += _vS * delta;
		_rD += _vR * delta;

		_theta = (_theta >= TWO_PI) ? _theta - TWO_PI : _theta;
		_sigma = (_sigma >= TWO_PI) ? _sigma - TWO_PI : _sigma;
		_rD = (_rD >= 360) ? _rD - 360 : _rD;

		_theta = (_theta <= -TWO_PI) ? _theta + TWO_PI : _theta;
		_sigma = (_sigma <= -TWO_PI) ? _sigma + TWO_PI : _sigma;
		_rD = (_rD <= -360) ? _rD + 360 : _rD;
		break;
	}
	case eStayMode:
	{
		break;
	}
	case eRotateToStay:
	{

		if (_animPos.hasFinishedAnimating() && _animPos.getPercentDone() == 1.0 &&
			_animRotate.hasFinishedAnimating() && _animRotate.getPercentDone() == 1.0)
		{
			_eState = eStayMode;
		}
		break;
	}
	case eStayToRotate:
	{
		if (_animPos.hasFinishedAnimating() && _animPos.getPercentDone() == 1.0 &&
			_animRotate.hasFinishedAnimating() && _animRotate.getPercentDone() == 1.0)
		{
			_eState = eRotateMode;
		}
		break;
	}
	}

}

//------------------------------------
ofVec3f textAnim::pUnit::getPos()
{
	ofVec3f result;
	switch (_eState)
	{
	case eRotateMode:
	{
		result = getRotatePos();
		break;
	}
	case eStayMode:
	{
		result = _stayPos;
		break;
	}
	case eRotateToStay:
	case eStayToRotate:
	{
		result = _animPos.getCurrentPosition();
		break;
	}

	}
	return result;
}

//------------------------------------
ofVec3f textAnim::pUnit::getRotateAxis()
{
	return _axis;
}

//------------------------------------
float textAnim::pUnit::getRotate()
{
	float result;
	switch (_eState)
	{
	case eRotateMode:
	{
		result = _rD;
		break;
	}
	case eStayMode:
	{
		result = 0;
		break;
	}
	case eRotateToStay:
	case eStayToRotate:
	{
		result = _animRotate.getCurrentValue();
		break;
	}

	}
	return result;
}

//------------------------------------
void textAnim::pUnit::toggle()
{
	switch (_eState)
	{
	case eRotateMode:
	{
		toStay();
	}
	case eStayMode:
	{
		toRotate();
	}
	}
}

//------------------------------------
void textAnim::pUnit::toStay()
{
	if (_eState == eRotateMode)
	{
		_animPos.setPosition(getRotatePos());
		_animRotate.reset(_rD);
		_animPos.animateTo(_stayPos);

		if (_rD > 180)
		{
			_animRotate.animateTo(360.0f);
		}
		else
		{
			_animRotate.animateTo(0);
		}
		_eState = eRotateToStay;
	}
}

//------------------------------------
void textAnim::pUnit::toRotate()
{
	if (_eState == eStayMode)
	{
		_animPos.setPosition(_stayPos);
		_animPos.animateTo(getRotatePos());
		_animRotate.animateTo(_rD);
		_eState = eStayToRotate;
	}
}

//------------------------------------
ofVec3f textAnim::pUnit::getRotatePos()
{
	float x = _r * sin(_theta) * cos(_sigma);
	float y = _r * sin(_theta) * sin(_sigma);
	float z = _r * cos(_theta);
	return ofVec3f(x, y, z);
}
#pragma endregion


#pragma region textAnim
//------------------------------------
void textAnim::set(text & newText)
{
	_text = newText;
	_moveList.clear();
	for (int j = 0; j < _text.getPartNum(); j++)
	{
		_moveList.push_back(pUnit(cTextFontSize * 1.2f, _text.getPos(j)));
	}

	_animGlowAlpha.setDuration(cTextLightAnimT);
	_animGlowAlpha.setRepeatType(AnimRepeat::LOOP_BACK_AND_FORTH);
	_animGlowAlpha.setCurve(AnimCurve::EASE_IN_EASE_OUT);
	_eState = eTextCode;

}

//------------------------------------
void textAnim::update(float delta)
{
	_animGlowAlpha.update(delta);
	for (auto& iter : _moveList)
	{
		iter.update(delta);
	}
}

//------------------------------------
void textAnim::draw()
{
	for (int i = 0; i < _text.getPartNum(); i++)
	{
		ofPushMatrix();
		ofTranslate(_moveList[i].getPos());

		auto axis = _moveList[i].getRotateAxis();
		ofRotate(_moveList[i].getRotate(), axis.x, axis.y, axis.z);
		_text.drawPart(i);
		ofPopMatrix();
	}
}

//------------------------------------
void textAnim::drawGlow()
{
	if (_eState == eTextCode)
	{
		return;
	}
	else
	{	
		ofPushStyle();
		ofSetColor(255, _animGlowAlpha.getCurrentValue());
		for (int i = 0; i < _text.getPartNum(); i++)
		{
			ofPushMatrix();
			ofTranslate(_moveList[i].getPos());


			auto axis = _moveList[i].getRotateAxis();
			ofRotate(_moveList[i].getRotate(), axis.x, axis.y, axis.z);
			_text.drawPart(i);
			ofPopMatrix();
		}
		ofPopStyle();
	}
}

//------------------------------------
void textAnim::trigger()
{
	switch (_eState)
	{
	case eTextCode:
	{
		_eState = eTextLightOn;
		_animGlowAlpha.animateTo(255);

		break;
	}
	case eTextLightOn:
	{
		_eState = eTextDisplayPart;
		int partNum = (int)floor((_moveList.size() / 2.0f));
		partNum = MAX(partNum, 1);

		for (int i = 0; i < partNum; i++)
		{
			_moveList[i].toStay();
		}
		break;
	}
	case eTextDisplayPart:
	{
		_eState = eTextDisplayAll;
		for (int i = 0; i < _moveList.size(); i++)
		{
			_moveList[i].toStay();
		}
		break;
	}
	case eTextDisplayAll:
	{
		_eState = eTextCode;
		for (int i = 0; i < _moveList.size(); i++)
		{
			_moveList[i].toRotate();
		}
		_animGlowAlpha.reset(0);
		break;
	}
	}
}
#pragma endregion


