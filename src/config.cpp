#include "config.h"

//--------------------------
void config::init()
{
	initGUI();
	loadConfig();
	loadGuiSetting();
}

//--------------------------
void config::save()
{
	_gui.saveToFile("_guiConfig.xml");
	ofLog(OF_LOG_NOTICE, "[config::save]Save config data");
}

//--------------------------
void config::draw()
{
	_fps = ofToString(ofGetFrameRate());
	_gui.draw();
}

//--------------------------
void config::initGUI()
{
	_gui.setup("", "_guiConfig.xml");

	_gui.add(_triggerW.setup("Trigger Width(mm)", 50, 50, 500));
	_gui.add(_triggerH.setup("Trigger Height(mm)", 200, 100, 2000));
	_gui.add(_triggerDist.setup("Trigger Distance(mm)", 200, 100, 1500));
	_gui.add(_updateTrigger.setup("Update Trigger"));
	_gui.add(_transitionT.setup("Transition Time", 3.0f, 1.0f, 5.0f));
	_gui.add(_fps.setup("FPS", "0.0"));

}

//--------------------------
void config::loadConfig()
{
	ofxXmlSettings xml;
	if (!xml.load("_config.xml"))
	{
		ofLog(OF_LOG_WARNING, "[config::loadConfig]load config xml failed");
	}

	_exEnableTestMode = xml.getValue("EnableTestMode", false);
	_exWindowWidth = xml.getValue("ScreenWidth", 1920);
	_exWindowHeight = xml.getValue("ScreenHeight", 1080);
	_exUrgPort = xml.getValue("UrgPort", 10940);
	_exUrgMM2Pix = xml.getValue("UrgMM2Pix", 0.2f);
	_exBGSphereNum = xml.getValue("BGSphereNum", 3000);

	_exUrgIP = xml.getValue("UrgIP", "192.168.1.10");

	_exMaxPort = xml.getValue("MaxPort", 7400);
	_exMaxIP = xml.getValue("MaxIP", "127.0.0.1");


}

//--------------------------
void config::loadGuiSetting()
{
	_gui.loadFromFile("_guiConfig.xml");
}


#pragma region Singleton
config* config::pInstance = 0;
config * config::getInstance()
{
	if (pInstance == 0)
	{
		pInstance = new config();
	}
	return pInstance;
}

void config::destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion