#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	text2Model::getInstance()->load("font.ttf", "fontUC.ttc");

	_textMeshList = text2Model::getInstance()->translate(L"ด๚ธี", 10.0f);
	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	_cam.begin();
	ofPushMatrix();
	for (auto iter : _textMeshList)
	{
		iter.draw();
	}
	
	ofPopMatrix();
	_cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
