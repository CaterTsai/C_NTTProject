#include "wordUnit.h"
#include "text2Model.h"

#pragma region pUnit
//------------------------------------
wordUnit::pUnit::pUnit()
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
wordUnit::pUnit::pUnit(float r, ofVec3f pos)
	:_r(r)
	,_eState(eRotateMode)
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
void wordUnit::pUnit::update(float delta)
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
ofVec3f wordUnit::pUnit::getPos()
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
ofVec3f wordUnit::pUnit::getRotateAxis()
{
	return _axis;
}

//------------------------------------
float wordUnit::pUnit::getRotate()
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
void wordUnit::pUnit::toggle()
{
	switch (_eState)
	{
	case eRotateMode:
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
		break;
	}
	case eStayMode:
	{
		_animPos.setPosition(_stayPos);
		_animPos.animateTo(getRotatePos());
		_animRotate.animateTo(_rD);
		_eState = eStayToRotate;
		break;
	}
	}
}

//------------------------------------
ofVec3f wordUnit::pUnit::getRotatePos()
{
	float x = _r * sin(_theta) * cos(_sigma);
	float y = _r * sin(_theta) * sin(_sigma);
	float z = _r * cos(_theta);
	return ofVec3f(x, y, z);
}
#pragma endregion

#pragma region wordUnit

//------------------------------------
void wordUnit::setup(wstring word)
{
	vector<ofVboMesh> _meshList;
	vector<int> _meshGroup;
	text2Model::getInstance()->translate(word, 3, _meshList, _meshGroup);

	int sIdx = 0;
	_textMgr.resize(_meshGroup.size());

	for (int i = 0; i < _meshGroup.size(); i++)
	{
		textUnit unit;
		unit.init(vector<ofVboMesh>(_meshList.begin() + sIdx, _meshList.begin() + _meshGroup[i]));
		sIdx = _meshGroup[i];
		_textMgr[i] = unit;

		vector<pUnit> pList;
		for (int j = 0; j < unit.getPartNum(); j++)
		{
			pList.push_back(pUnit(cTextFontSize * 1.2f, unit.getPos(j)));
		}
		_movePoint.push_back(pList);

	}
	_sphere.set(cTextFontSize * 1.5, 3);
	_isSetup = true;
}

//------------------------------------
void wordUnit::update(float delta)
{
	for (auto& moveSet : _movePoint)
	{
		for (auto& iter : moveSet)
		{
			iter.update(delta);
		}
	}
}

//------------------------------------
void wordUnit::draw(int x, int y, int z)
{
	ofPushStyle();
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(x, y, z);

	int index = 0;
	for (int i = 0; i < _textMgr.size(); i++)
	{
		for (int j = 0; j < _textMgr[i].getPartNum(); j++)
		{
			ofPushMatrix();
			ofTranslate(_movePoint[i][j].getPos());


			auto axis = _movePoint[i][j].getRotateAxis();
			ofRotate(_movePoint[i][j].getRotate(), axis.x, axis.y, axis.z);
			_textMgr[i].drawPart(j);
			ofPopMatrix();
		}
	}

	ofSetColor(255, 20);
	_sphere.drawWireframe();

	ofPopMatrix();
	ofPopStyle();
}

//------------------------------------
int wordUnit::getTextNum()
{
	return _textMgr.size();
}

//------------------------------------
void wordUnit::toggleText(int idx)
{
	if (idx >= 0 && idx < getTextNum())
	{
		for (auto& iter : _movePoint[idx])
		{
			iter.toggle();
		}
	}
}


#pragma endregion
