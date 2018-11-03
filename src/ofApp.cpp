#include "ofApp.h"
#include "text2Model.h"
//--------------------------------------------------------------
void ofApp::setup(){

	text2Model::getInstance()->load("font.ttf", "fontUC.ttc");
	//addWords();
	_wordUnit.setup(L"�����\");
	ofBackground(0);

	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	//for (int i = 0; i < cTextDisplaySize; i++)
	//{
	//	_wordList[i].update(delta);
	//}
	_wordUnit.update(delta);
}

//--------------------------------------------------------------
void ofApp::draw(){

	//for (int i = 0; i < cTextDisplaySize; i++)
	//{
	//	_wordList[i].draw(_wordPos[i].x, _wordPos[i].y, 0);
	//}
	_wordUnit.draw(ofGetWindowWidth() * 0.5f, ofGetWindowHeight() * 0.5f);
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 0, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'q':
	{
		_wordUnit.toggleText(0);
		break;
	}
	case 'w': 
	{
		_wordUnit.toggleText(1);
		break;
	}
	case 'e':
	{
		_wordUnit.toggleText(2);
		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::addWords()
{
	_wordList.resize(10);
	_wordList[0].setup(L"���v");
	_wordList[1].setup(L"�ʳf");
	_wordList[2].setup(L"�ڤh��");
	_wordList[3].setup(L"���B");
	_wordList[4].setup(L"�H�s");
	_wordList[5].setup(L"�d�_�K");
	_wordList[6].setup(L"�զ���");
	_wordList[7].setup(L"�o��");
	_wordList[8].setup(L"�H�e");
	_wordList[9].setup(L"���S��");

	float unitW = ofGetWindowWidth() / 11.0f;
	float x = unitW;
	_wordPos.resize(10);

	float noiseV = ofRandom(1.0);
	float delta = 0.01;
	for (int i = 0; i < 10; i++)
	{
		int y = ofGetWindowHeight() / 2.0f;
		y += (0.5 - ofNoise(i, noiseV)) * 400;
		_wordPos[i].set(x, y);
		x += unitW;
		noiseV += delta;
	}

}
