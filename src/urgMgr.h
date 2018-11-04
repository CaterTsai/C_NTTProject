#pragma once

#include "constParameter.h"

class urgMgr
{
#pragma region Trigger Area
private:
	class triggerArea
	{
	public:
		triggerArea();
		triggerArea(ofRectangle area, string id);

		void draw(float ratio);
		void check(ofVec2f pos);
		void clear();

	private:
		bool _isTrigger;
		ofRectangle _rect;
		string _id;
		int _checkCounter;
	};
#pragma endregion


public:
	urgMgr();
	void setup(string ip, int port, float mm2pix = 0.5f);
	void update(float delta);
	void draw(int x, int y);

	void start();
	void stop();

	void addTriggerArea(string id, int x, int y, int width, int height);
	void drawArea();

private:
	void checkTrigger();
	ofVec2f getPos(int index, long dist);
private:
	bool _isSetup;

	int _startDeg, _endDeg;
	float _mm2pix;
	float _degOfUnit;
	vector<long> _distData;
	vector<triggerArea> _triggerAreaList;
};