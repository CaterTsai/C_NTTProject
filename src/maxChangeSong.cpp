#include "maxChangeSong.h"

void maxChangeSong::init(string ip, int port)
{
	_ip = ip;
	_port = port;

	_sender.setup(_ip, port);
	_isInit = true;
}

void maxChangeSong::change(int i)
{
	ofxOscMessage msg;
	msg.setAddress("/oscVal");
	msg.addIntArg(i);

	_sender.sendMessage(msg, false);
}
