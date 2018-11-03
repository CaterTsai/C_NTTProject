#pragma once

#include "textUnit.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableFloat.h"
class wordUnit
{
#pragma region pUnit
private:
	class pUnit
	{
	public:
		pUnit();
		pUnit(float r, ofVec3f pos);
		void update(float delta);
		ofVec3f getPos();
		
		ofVec3f getRotateAxis();
		float getRotate();

		void toggle();

	private:
		enum eAnimState
		{
			eRotateMode = 0,
			eRotateToStay,
			eStayMode,
			eStayToRotate
		}_eState;

		//Rotate Mode
		float _theta, _sigma, _vT, _vS;
		float _r;
		ofVec3f _axis;
		float _rD, _vR;

		//Stay Mode
		ofVec3f _stayPos;

	private:
		ofVec3f getRotatePos();
		//Animation
		ofxAnimatableOfPoint	_animPos;
		ofxAnimatableFloat _animRotate;

	};
#pragma endregion


public:
	wordUnit()
		:_isSetup(false)
	{};

	void setup(wstring word);
	void update(float delta);
	void draw(int x, int y, int z = 0);

	int getTextNum();
	void toggleText(int idx);

private:
	bool _isSetup;
	ofIcoSpherePrimitive _sphere;
	vector<textUnit> _textMgr;
	vector<vector<pUnit>> _movePoint;
};