#include "projectionCanvas.h"

void projectionCanvas::init(int x, int y, int width, int height, int rowNum, int colNum)
{
	_canvas.allocate(width, height, GL_RGBA);
	_canvas.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
	float unitW = width / (float)(colNum - 1);
	float unitH = height / (float)(rowNum - 1);

	_mesh.clear();
	_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLES);
	_mesh.enableTextures();
	
	for (int sy = 0; sy < rowNum; sy++)
	{
		for (int sx = 0; sx < colNum; sx++)
		{
			_mesh.addVertex(ofVec2f(x + sx * unitW, y + sy * unitH));
			_mesh.addTexCoord(ofVec2f(sx * unitW, sy * unitH));
		}
	}

	for (int sy = 1; sy < rowNum; sy++)
	{
		for (int sx = 1; sx < colNum; sx++)
		{
			int a = (sy - 1) * colNum + sx - 1;
			int b = a + 1;
			int c = a + rowNum;
			int d = c + 1;
			_mesh.addTriangle(a, b, c);			
			_mesh.addTriangle(b, d, c);
		}
	}

	_isInit = true;
}

void projectionCanvas::draw()
{
	if (!_isInit || _isBegin)
	{
		return;
	}

	_canvas.getTextureReference().bind();
	_mesh.draw();
	_canvas.getTextureReference().unbind();
}

void projectionCanvas::drawMesh()
{
	if (!_isInit || _isBegin)
	{
		return;
	}

	ofSetColor(255);
	_mesh.drawWireframe();
}

void projectionCanvas::begin()
{
	if (!_isInit || _isBegin)
	{
		return;
	}
	_canvas.begin();
	ofClear(255);
	_isBegin = true;
}

void projectionCanvas::end()
{
	if (!_isInit || !_isBegin)
	{
		return;
	}
	_canvas.end();
	_isBegin = false;
}
