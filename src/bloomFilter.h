#pragma once

#include "ofxPostProcessing.h"

class bloomFilter
{
public:
	void init(int w, int h, bool arb);
	void filterEnable();
	void filterDisable();
private:
	bool _isInit;

public:
	ofxPostProcessing _bloom;
	//-------------------
	//Singleton
	//-------------------
private:
	bloomFilter()
		:_isInit(false)
	{};
	~bloomFilter()
	{
		bloomFilter::Destroy();
	}
	bloomFilter(bloomFilter const&);
	void operator=(bloomFilter const&);

public:
	static bloomFilter* GetInstance();
	static void Destroy();

private:
	static bloomFilter *pInstance;
};