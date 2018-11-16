#pragma once

#include "constParameter.h"
#include "ofxOsc.h"

class maxSender
{
public:
	maxSender()
		:_isInit(false)
	{};
	void init(string ip, int port);

	void trigger(int i);
	void off();

private:
	bool _isInit;
	ofxOscSender _sender;
	ofxOscMessage _trigger[cTriggerStateNum];
	ofxOscMessage _off;
	string _ip;
	int _port;
};
