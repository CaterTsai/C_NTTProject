#pragma once

#include "text.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableFloat.h"

enum eTextState : int
{
	eTextOut = 0,
	eTextCode,
	eTextCode2LightOn,
	eTextLightOn,
	eTextLightOn2DisplayPart,
	eTextDisplayPart,
	eTextDisplayPart2All,
	eTextDisplayAll,
	eTextExplode,

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
		void toOut();

	public:
		enum eAnimState
		{
			eRotateMode = 0,
			eRotateToStay,
			eStayMode,
			eStayToOut,
			eOutMode
		}_eState;
	private:
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
	textAnim()
		:_isSet(false)
		, _eState(eTextCode)
	{}
	void init() {};
	void set(text& newText);
	void clear();

	void update(float delta);
	void draw();
	void drawGlow();

	void trigger();
	void explode();

private:
	void checkAnim();

public:
	eTextState _eState;

private:
	bool _isSet;
	ofxAnimatableFloat _animGlowAlpha;
	text _text;
	vector<pUnit> _moveList;
};