#pragma once

#include "constParameter.h"
#include "ofxTrueTypeFontUC.h"

class text2Model
{
public:
	void load(string path);
	void translate(string text);

private:
	bool _isLoad;
	ofFbo _canvas;
	ofxTrueTypeFontUC	_font;
//-------------------
//Singleton
//-------------------
private:
	text2Model()
		:_isLoad(false)
	{};
	~text2Model()
	{
		text2Model::destroy();
	}
	text2Model(text2Model const&);
	void operator=(text2Model const&);

public:
	static text2Model* getInstance();
	static void destroy();

private:
	static text2Model *pInstance;


};