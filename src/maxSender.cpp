#include "maxSender.h"

//------------------------------------------
void maxSender::init(string ip, int port)
{
	_ip = ip;
	_port = port;
	_on.setAddress("/oscVal");
	_on.addBoolArg(true);
	_off.setAddress("/oscVal");
	_off.addBoolArg(false);
	
	_sender.setup(_ip, port);
	_isInit = true;
}

//------------------------------------------
void maxSender::setOn()
{
	if (!_isInit)
	{
		return;
	}
	_sender.sendMessage(_on, false);
}

//------------------------------------------
void maxSender::setOff()
{
	if (!_isInit)
	{
		return;
	}
	_sender.sendMessage(_off, false);
}
