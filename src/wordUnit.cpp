#include "wordUnit.h"
#include "text2Model.h"

//------------------------------------
void wordUnit::setup(wstring word)
{
	vector<ofVboMesh> _meshList;
	vector<int> _meshGroup;
	text2Model::getInstance()->translate(word, 10.0f, _meshList, _meshGroup);

	int sIdx = 0;
	_textMgr.resize(_meshGroup.size());
	for (int i = 0; i < _meshGroup.size(); i++)
	{
		textUnit unit;
		unit.init(vector<ofVboMesh>(_meshList.begin() + sIdx, _meshList.begin() + _meshGroup[i]));
		sIdx = _meshGroup[i];

		_textMgr[i] = unit;
	}

	_isSetup = true;
}

//------------------------------------
void wordUnit::update(float delta)
{
}

//------------------------------------
void wordUnit::draw(int x, int y, int z)
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(x, y, z);

	for (auto& iter : _textMgr)
	{
		for (int i = 0; i < iter.getPartNum(); i++)
		{
			iter.drawPart(i);
		}
	}
	ofPopMatrix();
	ofPopStyle();
}
