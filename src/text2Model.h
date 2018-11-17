#pragma once

#include "constParameter.h"
#include "ofxTrueTypeFontUC.h"

class text2Model
{
public:
	void load(string ucPath);
	void translate(wstring text, float depth, vector<ofVboMesh>& meshList, vector<int>& groupList);
	void translate(string text, float depth, vector<ofVboMesh>& meshList, vector<int>& groupList);

public:

	string ws2s(wstring wstr);
	wstring s2ws(const string & str);

private:
	bool _isLoad;
	ofxTrueTypeFontUC	_fontUC;
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