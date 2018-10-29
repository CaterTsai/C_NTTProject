#pragma once

#include "constParameter.h"


class textUnit
{
public:
	textUnit():_isInit(false){};
	void init(vector<ofVboMesh>& meshList);
	void drawPart(int pIdx);
	void drawFace();
	int getPartNum();
private:
	ofVec3f rebuild(ofVboMesh& mesh);

private:
	bool _isInit;
	ofVboMesh _face;
	vector<ofVboMesh> _part;
	vector<ofVec3f> _tempPos;
};