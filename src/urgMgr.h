#pragma once

#include "constParameter.h"
#include "ofxAnimatableOfPoint.h"

class urgMgr
{
#pragma region Trigger Area
public:
	class triggerArea
	{
	public:
		triggerArea();
		triggerArea(ofRectangle area, string id);

		void draw(float ratio);
		void check(ofVec2f pos);
		
		void finish();
		void clear();

		void testCheck(ofRectangle rect);
		void updateArea(ofRectangle rect);

	private:
		bool _isTrigger, _triggerTmp;
		ofRectangle _rect;
		string _id;
		int _checkCounter;

	public:
		//Global Event
		static ofEvent<string> _onTriggerOn;
		static ofEvent<string> _onTriggerOff;

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
	void addListTriggerArea(int num, int dist, int width, int hegiht);
	void updateTriggerArea(int dist, int width, int height);
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

#pragma region Test Mode
public:
	void testUpdate(float delta);
	void testDraw(int x, int y);
	void addTestPoint(ofVec2f s, ofVec2f e, float duration);
	void clearTestPoint();
private:
	vector<ofxAnimatableOfPoint> _testPoint;
	vector<ofRectangle> _testRect;
	float _testRectSize;
#pragma endregion

	

};