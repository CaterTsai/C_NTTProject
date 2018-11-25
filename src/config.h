#pragma once

#include "ofxXmlSettings.h"
#include "ofxGui.h"

class config
{
public:
	void init();
	void save();
	void draw();

private:
	void initGUI();
	void loadConfig();
	void loadGuiSetting();

public:

	//Config
	bool _exEnableTestMode;
	int _exWindowWidth, _exWindowHeight;
	int _exUrgPort;
	int _exBGSphereNum;
	int _exMaxPort;
	float _exUrgMM2Pix;
	string _exUrgIP;
	string _exMaxIP;


	//Gui
	ofxPanel _gui;
	ofxFloatSlider _transitionT, _explodeWaitT;
	ofxIntSlider _triggerW, _triggerH, _triggerDist;
	ofxButton _updateTrigger;
	ofxLabel _fps;

	//-------------------
	//Singleton
	//-------------------
private:
	config()
	{
	};
	~config()
	{
		config::destroy();
	}
	config(config const&);
	void operator=(config const&);

public:
	static config* getInstance();
	static void destroy();

private:
	static config *pInstance;
};