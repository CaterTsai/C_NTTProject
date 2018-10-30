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
{}

//------------------------------------
wordUnit::pUnit::pUnit(float r)
	:_r(r)
{
	_theta = ofRandom(0, PI);
	_sigma = ofRandom(0, TWO_PI);

	_vT = ofRandom(-TWO_PI, TWO_PI) * 0.1f;
	_vS = ofRandom(-TWO_PI, TWO_PI) * 0.1f;

	_axis.set(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
	_axis.normalize();

	_rD = ofRandom(0, 360);
	_vR = ofRandom(-360, 360) * 0.1f;
}

//------------------------------------
void wordUnit::pUnit::update(float delta)
{
	_theta += _vT * delta;
	_sigma += _vS * delta;
	_rD += _vR * delta;

	_theta = (_theta >= TWO_PI) ? _theta - TWO_PI : _theta;
	_sigma = (_sigma >= TWO_PI) ? _sigma - TWO_PI : _sigma;
	_rD = (_rD >= 360) ? _rD - 360 : _rD;

	_theta = (_theta <= -TWO_PI) ? _theta + TWO_PI : _theta;
	_sigma = (_sigma <= TWO_PI) ? _sigma + TWO_PI : _sigma;
	_rD = (_rD <= 360) ? _rD + 360 : _rD;
}

//------------------------------------
ofVec3f wordUnit::pUnit::getPos()
{
	float x = _r * sin(_theta) * cos(_sigma);
	float y = _r * sin(_theta) * sin(_sigma);
	float z = _r * cos(_theta);
	return ofVec3f(x, y, z);
}
#pragma endregion

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
		for (int j = 0; j < unit.getPartNum(); j++)
		{
			_movePoint.push_back(pUnit(cTextFontSize * 1.2));
		}
		
	}
	_sphere.set(cTextFontSize * 1.5, 3);
	_isSetup = true;
}

//------------------------------------
void wordUnit::update(float delta)
{
	for (auto& iter : _movePoint)
	{
		iter.update(delta);
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
	for (auto& iter : _textMgr)
	{
		for (int i = 0; i < iter.getPartNum(); i++)
		{
			ofPushMatrix();
			ofTranslate(_movePoint[index].getPos());
			ofRotate(_movePoint[index]._rD, _movePoint[index]._axis.x, _movePoint[index]._axis.y, _movePoint[index]._axis.z);
			iter.drawPart(i);
			ofPopMatrix();

			index++;
		}
	}

	ofSetColor(255, 20);
	_sphere.drawWireframe();

	
	ofPopMatrix();
	ofPopStyle();
}

