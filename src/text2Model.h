#pragma once

#include "constParameter.h"
#include "ofxTrueTypeFontUC.h"

class text2Model
{
public:
	void load(string path, string ucPath);
	void translate(wstring text, float depth, vector<ofVboMesh>& meshList, vector<int>& groupList);
	vector<ofVboMesh> translate(string text, float depth);

private:

	string ws2s(wstring wstr);
	wstring s2ws(const string & str);

private:
	bool _isLoad;
	ofxTrueTypeFontUC	_fontUC;
	ofTrueTypeFont _font;
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