#pragma once

#include "constParameter.h"
#include "text2Model.h"


class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	ofEasyCam _cam;
	vector<ofVboMesh> _textMeshList;

};
