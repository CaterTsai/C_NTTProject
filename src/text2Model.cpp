#include "text2Model.h"
#include "config.h"
//--------------------------------------------------------------
void text2Model::load(string ucPath)
{
	_fontUC.setGlobalDpi(72);
	if (!_fontUC.load(ucPath, config::getInstance()->_exFontSize, true, true))
	{
		ofLog(OF_LOG_ERROR, "[text2Model::load]Cant load fontUC");
		return;
	}

	_isLoad = true;

}

//--------------------------------------------------------------
void text2Model::translate(wstring text, float depth, vector<ofVboMesh>& meshList, vector<int>& groupList)
{
	meshList.clear();
	groupList.clear();
	string word = ws2s(text);

	auto bounding = _fontUC.getStringBoundingBox(word, 0, 0);
	ofVec3f center((bounding.width) * 0.5f + bounding.x, bounding.y * 0.5, 0);
	vector <ofPath> word_paths = _fontUC.getStringAsPoints(word, 0);
	int index = 0;
	for (int i = 0; i < word_paths.size(); i++) {

		ofVboMesh current_char_mesh;

		
		vector <ofPolyline> char_polylines = word_paths.at(i).getOutline();
		index += char_polylines.size() + 1;
		groupList.push_back(index);

		ofVboMesh front; 
		ofPath front_path; 
		for (int c = 0; c < char_polylines.size(); c++) {
			for (int p = 0; p < char_polylines[c].size(); p++) {

				if (p == 0) {
					front_path.moveTo(char_polylines[c][p] - center);
				}
				else {
					front_path.lineTo(char_polylines[c][p] - center);
				}
			}
		}
		front = front_path.getTessellation();
		ofVec3f * front_vertices = front.getVerticesPointer();
		ofVboMesh back = front;

		for (int v = 0; v < front.getNumVertices(); v++) {
			front_vertices[v].z += depth;
		}

		current_char_mesh.append(front);
		current_char_mesh.append(back);

		meshList.push_back(current_char_mesh);

		
		vector <ofPolyline> lines = word_paths.at(i).getOutline();
		for (int j = 0; j < lines.size(); j++) {

			ofVboMesh side;
			vector <ofPoint> points = lines.at(j).getVertices();
			int k = 0;

			for (k = 0; k < points.size() - 1; k++) {
				ofPoint p1 = points.at(k + 0) - center;
				ofPoint p2 = points.at(k + 1) - center;

				side.addVertex(p1);
				side.addVertex(p2);
				side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));
				side.addVertex(ofPoint(p2.x, p2.y, p2.z + depth));
				side.addVertex(p2);
			}

			// connect the last to the first
			ofPoint p1 = points.at(k) - center;
			ofPoint p2 = points.at(0) - center;
			side.addVertex(p1);
			side.addVertex(p2);
			side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));

			side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));
			side.addVertex(ofPoint(p2.x, p2.y, p2.z + depth));
			side.addVertex(p2);

			side.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

			meshList.push_back(side);
		}
	}
}

//--------------------------------------------------------------
void text2Model::translate(string word, float depth, vector<ofVboMesh>& meshList, vector<int>& groupList)
{
	meshList.clear();
	groupList.clear();

	auto bounding = _fontUC.getStringBoundingBox(word, 0, 0);
	ofVec3f center((bounding.width) * 0.5f + bounding.x, bounding.y * 0.5, 0);
	vector <ofPath> word_paths = _fontUC.getStringAsPoints(word, 0);
	int index = 0;
	for (int i = 0; i < word_paths.size(); i++) {

		ofVboMesh current_char_mesh;


		vector <ofPolyline> char_polylines = word_paths.at(i).getOutline();
		index += char_polylines.size() + 1;
		groupList.push_back(index);

		ofVboMesh front;
		ofPath front_path;
		for (int c = 0; c < char_polylines.size(); c++) {
			for (int p = 0; p < char_polylines[c].size(); p++) {

				if (p == 0) {
					front_path.moveTo(char_polylines[c][p] - center);
				}
				else {
					front_path.lineTo(char_polylines[c][p] - center);
				}
			}
		}
		front = front_path.getTessellation();
		ofVec3f * front_vertices = front.getVerticesPointer();
		ofVboMesh back = front;

		for (int v = 0; v < front.getNumVertices(); v++) {
			front_vertices[v].z += depth;
		}

		current_char_mesh.append(front);
		current_char_mesh.append(back);

		meshList.push_back(current_char_mesh);


		vector <ofPolyline> lines = word_paths.at(i).getOutline();
		for (int j = 0; j < lines.size(); j++) {

			ofVboMesh side;
			vector <ofPoint> points = lines.at(j).getVertices();
			int k = 0;

			for (k = 0; k < points.size() - 1; k++) {
				ofPoint p1 = points.at(k + 0) - center;
				ofPoint p2 = points.at(k + 1) - center;

				side.addVertex(p1);
				side.addVertex(p2);
				side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));
				side.addVertex(ofPoint(p2.x, p2.y, p2.z + depth));
				side.addVertex(p2);
			}

			// connect the last to the first
			ofPoint p1 = points.at(k) - center;
			ofPoint p2 = points.at(0) - center;
			side.addVertex(p1);
			side.addVertex(p2);
			side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));

			side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));
			side.addVertex(ofPoint(p2.x, p2.y, p2.z + depth));
			side.addVertex(p2);

			side.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

			meshList.push_back(side);
		}
	}
}

//--------------------------------------------------------------
string text2Model::ws2s(wstring wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, 0);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, 0);
	return strTo;

}

//--------------------------------------------------------------
wstring text2Model::s2ws(const string & str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
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