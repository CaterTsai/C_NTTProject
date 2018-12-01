#include "textAnim.h"
#include "config.h"
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

	_vT = ofRandom(-TWO_PI, TWO_PI) * 0.2f;
	_vS = ofRandom(-TWO_PI, TWO_PI) * 0.2f;

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
	case eStayToOut:
	{
		if (_animPos.hasFinishedAnimating() && _animPos.getPercentDone() == 1.0)
		{
			_eState = eOutMode;
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
	case eOutMode:
	{
		result = _animPos.getCurrentPosition();
		break;
	}
	case eRotateToStay:
	case eStayToOut:
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
	float result = 0.0f;
	switch (_eState)
	{
	case eRotateMode:
	{
		result = _rD;
		break;
	}
	case eRotateToStay:
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
		toOut();
	}
	}
}

//------------------------------------
void textAnim::pUnit::toStay()
{
	if (_eState == eRotateMode)
	{
		_animPos.setPosition(getRotatePos());
		_animPos.setDuration(cTextAnimT);
		_animPos.setCurve(AnimCurve::EASE_IN_EASE_OUT);

		_animRotate.setDuration(cTextAnimT * 1.2);
		_animRotate.setCurve(AnimCurve::EASE_IN_EASE_OUT);
		
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
void textAnim::pUnit::toOut()
{
	if (_eState == eStayMode)
	{
		ofVec3f v = _stayPos.normalized();
		v.rotate(ofRandom(-45, 45), v.rotated(-90, ofVec3f(0, 0, 1)));
		v *= cTextOutLevel;
		_animPos.setCurve(AnimCurve::EASE_IN_BACK);
		_animPos.animateTo(v);
		_animPos.setDuration(cTextOutAnimT);
		_eState = eStayToOut;
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
	_animGlowAlpha.setCurve(AnimCurve::EASE_IN_EASE_OUT);
	_eState = eTextCode;
	_isSet = true;
	_canTrigger = true;
}

//------------------------------------
void textAnim::clear()
{
	_isSet = false;
}

//------------------------------------
void textAnim::update(float delta)
{
	if (!_isSet)
	{
		return;
	}
	_animGlowAlpha.update(delta);
	for (auto& iter : _moveList)
	{
		iter.update(delta);
	}
	checkAnim();
}

//------------------------------------
void textAnim::draw()
{
	if (!_isSet)
	{
		return;
	}
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
	if (!_isSet)
	{
		return;
	}
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
	if (!_isSet || !_canTrigger)
	{
		return;
	}
	switch (_eState)
	{
	case eTextCode:
	{
		_eState = eTextCode2LightOn;
		_animGlowAlpha.animateFromTo(255, config::getInstance()->_glowTextLevel);
		_canTrigger = false;
		break;
	}
	case eTextLightOn:
	{
		_eState = eTextLightOn2DisplayPart;
		int partNum = (int)floor((_moveList.size() / 2.0f));
		partNum = MAX(partNum, 1);

		for (int i = 0; i < partNum; i++)
		{
			_moveList[i].toStay();
		}
		_animGlowAlpha.animateFromTo(255, config::getInstance()->_glowTextLevel);
		_canTrigger = false;
		break;
	}
	case eTextDisplayPart:
	{
		_eState = eTextDisplayPart2All;
		for (int i = 0; i < _moveList.size(); i++)
		{
			_moveList[i].toStay();
		}
		_animGlowAlpha.animateFromTo(255, config::getInstance()->_glowTextLevel);
		_canTrigger = false;
		break;
	}
	}
}

//------------------------------------
void textAnim::explode()
{
	if (!_isSet)
	{
		return;
	}
	if (_eState == eTextDisplayAll)
	{
		_eState = eTextExplode;
		for (int i = 0; i < _moveList.size(); i++)
		{
			_moveList[i].toOut();
		}
		_animGlowAlpha.setDuration(cTextOutAnimT);
		_animGlowAlpha.animateFromTo(255, 0);
	}
}

//------------------------------------
void textAnim::setTrigger(bool val)
{
	_canTrigger = val;
}

//------------------------------------
void textAnim::checkAnim()
{
	switch (_eState)
	{
	case eTextCode2LightOn:
	{
		if (_animGlowAlpha.hasFinishedAnimating() && _animGlowAlpha.getPercentDone() == 1.0f)
		{
			_eState = eTextLightOn;
		}
		break;
	}
	case eTextLightOn2DisplayPart:
	{
		bool result = true;
		for (auto& iter : _moveList)
		{
			result &= (iter._eState != pUnit::eRotateToStay);
		}
		if (result)
		{
			_eState = eTextDisplayPart;
		}
		break;
	}
	case eTextDisplayPart2All:
	{
		bool result = true;
		for (auto& iter : _moveList)
		{
			result &= (iter._eState == pUnit::eStayMode);
		}
		if (result)
		{
			_eState = eTextDisplayAll;
		}
		break;
	}
	case eTextExplode:
	{
		bool result = true;
		for (auto& iter : _moveList)
		{
			result &= (iter._eState == pUnit::eOutMode);
		}
		if (result)
		{
			_eState = eTextOut;
		}
		break;
	}
	}
}
#pragma endregion


