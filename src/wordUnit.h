#pragma once

#include "text.h"
#include "textAnim.h"

class wordUnit
{
public:
	wordUnit()
		:_isSet(false)
		, _eState(eTextCode)
	{};

	void init();
	void addText(wstring word);
	void setText(int idx);
	void update(float delta);
	void draw(int x, int y, int z = 0);
	void drawGlow(int x, int y, int z = 0);

	int getTextNum();
	eTextState triggerText(int idx);

private:
	bool _isSet;
	int _textNum;
	array<textAnim, cTriggerEachGroup> _textAnimMgr;
	vector<vector<text>> _textMgr;

private:
	void initSphere();
	void updateSphere(float delta);
	void triggerSphere();
private:
	ofxAnimatableFloat _animSize, _animAlpha;
	ofSpherePrimitive _sphere;
	eTextState _eState;
};