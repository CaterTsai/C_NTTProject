#include "maxSender.h"

//------------------------------------------
void maxSender::init(string ip, int port)
{
	_ip = ip;
	_port = port;

	for (int i = 0; i < cTriggerStateNum; i++)
	{
		_trigger[i].setAddress("/oscVal");
		_trigger[i].addIntArg(i + 1);
	}
	
	_off.setAddress("/oscVal");
	_off.addIntArg(0);
	
	_sender.setup(_ip, port);
	_isInit = true;
}

//------------------------------------------
void maxSender::trigger(int state)
{
	if (!_isInit || state == -1)
	{
		return;
	}
	_sender.sendMessage(_trigger[state], false);
}

//------------------------------------------
void maxSender::off()
{
	if (!_isInit)
	{
		return;
	}
	_sender.sendMessage(_off, false);
}
