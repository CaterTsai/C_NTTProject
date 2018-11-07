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

	void setOn();
	void setOff();

private:
	bool _isInit;
	ofxOscSender _sender;
	ofxOscMessage _on, _off;
	string _ip;
	int _port;
};
