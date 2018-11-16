#pragma once

#include "text.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableFloat.h"

enum eTextState : int
{
	eTextCode = 0,
	eTextLightOn,
	eTextDisplayPart,
	eTextDisplayAll,

	eTextUnknow = 100
};

class textAnim
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
		void toStay();
		void toRotate();

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
	void init() {};
	void set(text& newText);

	void update(float delta);
	void draw();
	void drawGlow();

	void trigger();

public:
	eTextState _eState;

private:
	ofxAnimatableFloat _animGlowAlpha;
	text _text;
	vector<pUnit> _moveList;
};