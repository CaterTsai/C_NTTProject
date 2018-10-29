#pragma once

#include "constParameter.h"
#include "wordUnit.h"


class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	ofEasyCam _cam;

	wordUnit _word;
};
