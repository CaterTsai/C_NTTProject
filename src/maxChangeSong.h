#pragma once

#include "constParameter.h"
#include "ofxOsc.h"

class maxChangeSong
{
public:
	maxChangeSong()
		:_isInit(false)
	{};
	void init(string ip, int port);
	void change(int i);

private:
	bool _isInit;
	ofxOscSender _sender;
	string _ip;
	int _port;
};
