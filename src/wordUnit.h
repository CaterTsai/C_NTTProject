#pragma once

#include "text.h"
#include "textAnim.h"

class wordUnit
{
public:
	wordUnit()
		:_isSet(false)
		, _eState(eTextCode)
		,_id(-1)
		, _needCheckAnim(false)
	{};

	void init(int id);
	void addText(wstring word);
	void addText(string word);
	void setText(int idx);
	void update(float delta);
	void draw(int x, int y, int z = 0);
	void drawGlow(int x, int y, int z = 0);
	bool isAllDisplay();
	bool isAllOut();
	int getTextNum();
	eTextState triggerText(int idx);
	void checkAnim();

	void explode();

private:
	int _id;
	bool _isSet;
	int _textNum;
	ofxAnimatableFloat _animTextAlpha;
	array<textAnim, cTriggerEachGroup> _textAnimMgr;
	vector<vector<text>> _textMgr;

#pragma region Sphere
//-----------------------------------
private:
	void initSphere();
	void updateSphere(float delta);
	void triggerSphere();
private:
	bool _glowSphere;
	ofxAnimatableFloat _animSize, _animAlpha;
	ofSpherePrimitive _sphere;
	eTextState _eState;
#pragma endregion
	
//-----------------------------------
public:
	//Global Event
	static ofEvent<int> _onWordDisplay;
	static ofEvent<int> _onWordOut;

private:
	bool _needCheckAnim;
};