#include "textUnit.h"

//-------------------------------
void textUnit::init(vector<ofVboMesh>& meshList)
{
	if (meshList.size() <= 0)
	{
		return;
	}
	_face = meshList[0];
	_part = vector<ofVboMesh>(meshList.begin() + 1, meshList.end());

	for (auto& iter : _part)
	{
		_tempPos.push_back(rebuild(iter));
	}
	_isInit = true;
}

//-------------------------------
void textUnit::drawPart(int pIdx)
{
	if (pIdx >= _part.size())
	{
		return;
	}
	_part[pIdx].drawWireframe();
}

//-------------------------------
void textUnit::drawFace()
{
	_face.draw();
}

//-------------------------------
int textUnit::getPartNum()
{
	return _part.size();
}

//-------------------------------
ofVec3f textUnit::getPos(int pIdx)
{
	if (pIdx >= _tempPos.size())
	{
		return ofVec3f(0);
	}
	return _tempPos[pIdx];
}

//-------------------------------
ofVec3f textUnit::rebuild(ofVboMesh & mesh)
{
	auto allVector = mesh.getVertices();
	ofVec3f center(0);
	for (auto& iter : allVector)
	{
		center += iter;
	}
	center /= mesh.getNumVertices();

	for (int i = 0; i < mesh.getNumVertices(); i++)
	{
		ofVec3f newV = allVector[i] - center;
		mesh.setVertex(i, newV);
	}

	return center;
}
