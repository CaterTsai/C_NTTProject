#pragma once

#include "constParameter.h"
#include "bloomFilter.h"

class text
{
public:
	text():_isInit(false){};
	void init(vector<ofVboMesh>& meshList);
	void drawPart(int pIdx);
	void drawFace();
	int getPartNum();
	ofVec3f getPos(int pIdx);
private:
	ofVec3f rebuild(ofVboMesh& mesh);

private:
	bool _isInit;
	ofVboMesh _face;
	vector<ofVboMesh> _part;
	vector<ofVec3f> _tempPos;
};