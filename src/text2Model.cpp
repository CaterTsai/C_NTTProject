#include "text2Model.h"

//--------------------------------------------------------------
void text2Model::load(string path)
{
	_font.setGlobalDpi(72);
	if (!_font.load(path, cTextCanvasSize))
	{
		ofLog(OF_LOG_ERROR, "[text2Model::load]Cant load font");
		return;
	}
	_isLoad = true;

}

//--------------------------------------------------------------
void text2Model::translate(string text)
{
}



#pragma region Singleton
//--------------------------------------------------------------
text2Model* text2Model::pInstance = 0;

text2Model* text2Model::getInstance()
{
	if (pInstance == 0)
	{
		pInstance = new text2Model();
	}
	return pInstance;
}

//--------------------------------------------------------------
void text2Model::destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion