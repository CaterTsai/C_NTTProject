#include "ofApp.h"
#include "text2Model.h"
//--------------------------------------------------------------
void ofApp::setup(){


	text2Model::getInstance()->load("font.ttf", "fontUC.ttc");
	_word.setup(L"¤@¤f");
	ofBackground(0);
	_cam.setVFlip(true);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	_cam.begin();
	_word.draw(0, 0);
	_cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
