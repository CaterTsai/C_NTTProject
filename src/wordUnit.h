#pragma once

#include "textUnit.h"

class wordUnit
{
#pragma region pUnit
private:
	class pUnit
	{
	public:
		pUnit();
		pUnit(float r);
		void update(float delta);
		
		ofVec3f getPos();
	public:
		float _theta, _sigma, _vT, _vS;
		float _r;
		ofVec3f _axis;
		float _rD, _vR;
	};
#pragma endregion


public:
	wordUnit()
		:_isSetup(false)
	{};

	void setup(wstring word);
	void update(float delta);
	void draw(int x, int y, int z = 0);

private:
	bool _isSetup;
	ofIcoSpherePrimitive _sphere;
	vector<textUnit> _textMgr;
	vector<pUnit> _movePoint;
};