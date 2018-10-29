#pragma once

#include "textUnit.h"

class wordUnit
{
public:
	wordUnit()
		:_isSetup(false)
	{};

	void setup(wstring word);
	void update(float delta);
	void draw(int x, int y, int z = 0);

private:
	bool _isSetup;
	vector<textUnit> _textMgr;
};