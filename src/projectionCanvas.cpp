#include "projectionCanvas.h"

void projectionCanvas::init(int id, int x, int y, int width, int height, int rowNum, int colNum)
{
	_canvasId = id;
	_canvas.allocate(width, height, GL_RGBA);
	_canvas.getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
	float unitW = width / (float)(colNum - 1);
	float unitH = height / (float)(rowNum - 1);
	_rowNum = rowNum;
	_colNum = colNum;
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

	load();
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

void projectionCanvas::load()
{
	if (!_isInit)
	{
		return;
	}

	ofxXmlSettings xml;
	if (!xml.load("_pConfig_" + ofToString(_canvasId) + ".xml"))
	{
		ofLog(OF_LOG_WARNING, "[projectionCanvas::load]Can't found _pConfig");
		return;
	}

	int num = xml.getNumTags("vertex");
	if (num != _mesh.getNumVertices())
	{
		ofLog(OF_LOG_WARNING, "[projectionCanvas::load]config vertex num is not match");
		return;
	}

	for (int i = 0; i < num; i++)
	{
		ofVec2f pos;
		xml.pushTag("vertex", i);
		pos.x = xml.getValue("X", 0);
		pos.y = xml.getValue("Y", 0);

		_mesh.setVertex(i, pos);
		xml.popTag();
	}

	ofLog(OF_LOG_NOTICE, "[projectionCanvas::load]Load config success");
}

void projectionCanvas::save()
{
	if (!_isInit)
	{
		return;
	}

	ofxXmlSettings xml;
	for (int i = 0; i < _mesh.getNumVertices(); i++)
	{
		xml.addTag("vertex");
		xml.pushTag("vertex", i);
		xml.addValue("X", _mesh.getVertex(i).x);
		xml.addValue("Y", _mesh.getVertex(i).y);
		xml.popTag();
	}
	xml.saveFile("_pConfig_" + ofToString(_canvasId) + ".xml");
	ofLog(OF_LOG_NOTICE, "[projectionCanvas::load]Save config success");
}


void projectionCanvas::debugUpdate()
{
	if (!_isInit || _isBegin || !_isDebugMode)
	{
		return;
	}

	_canvas.begin();
	{
		float unitW = _canvas.getWidth() / (float)(_colNum - 1);
		float unitH = _canvas.getHeight() / (float)(_rowNum - 1);

		ofPushStyle();
		ofFill();
		for (int sy = 0; sy < _colNum - 1; sy++)
		{
			bool flag = (sy % 2 == 0);
			for (int sx = 0; sx < _rowNum - 1; sx++)
			{
				(flag) ? ofSetColor(0, 255, 0, 100) : ofSetColor(0, 0, 255, 100);
				ofDrawRectangle(sx * unitW, sy * unitH, unitW, unitH);
				flag = !flag;
			}
		}
		ofPopStyle();
	}
	_canvas.end();
}

void projectionCanvas::debugDraw()
{
	if (!_isInit || !_isDebugMode)
	{
		return;
	}


	_canvas.getTextureReference().bind();
	_mesh.draw();
	_canvas.getTextureReference().unbind();

	ofPushStyle();
	ofSetColor(255);
	_mesh.drawWireframe();

	auto pos = _mesh.getVertex(_ctrlIdx);
	ofNoFill();
	ofSetLineWidth(3);
	ofSetColor(255, 0, 0);
	ofDrawCircle(pos, 10);

	ofPopStyle();
}

void projectionCanvas::setDebugMode(bool val)
{
	_isDebugMode = val;
}

bool projectionCanvas::getDebugMode()
{
	return _isDebugMode;
}

void projectionCanvas::debugCtrl(int key)
{
	switch (key) {
	case OF_KEY_LEFT: {
		if (_ctrlIdx % _colNum == 0)
		{
			_ctrlIdx += (_colNum - 1);
		}
		else
		{
			_ctrlIdx--;
		}
		break;
	}
	case OF_KEY_RIGHT: {
		if ((_ctrlIdx + 1) % _colNum == 0)
		{
			_ctrlIdx -= (_colNum - 1);
		}
		else
		{
			_ctrlIdx++;
		}
		break;
	}
	case OF_KEY_UP: {
		_ctrlIdx -= _colNum;
		if (_ctrlIdx < 0)
		{
			_ctrlIdx += (_mesh.getNumVertices());
		}
		break;
	}
	case OF_KEY_DOWN: {
		_ctrlIdx += _colNum;
		if (_ctrlIdx >= _mesh.getNumVertices())
		{
			_ctrlIdx -= (_mesh.getNumVertices());
		}
		break;
	}
	case 'p': {
		save();
		break;
	}
	}
}

void projectionCanvas::debugMouseCtrl(int x, int y)
{
	ofVec3f mousePos(x, y);
	auto pos = _mesh.getVertex(_ctrlIdx);

	if (mousePos.distance(pos) <= 50)
	{
		_mesh.setVertex(_ctrlIdx, mousePos);
	}
}
