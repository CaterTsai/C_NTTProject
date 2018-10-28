#include "text2Model.h"

//--------------------------------------------------------------
void text2Model::load(string path, string ucPath)
{
	_font.setGlobalDpi(72);
	if (!_font.load(path, cTextCanvasSize, true, true, true))
	{
		ofLog(OF_LOG_ERROR, "[text2Model::load]Cant load font");
		return;
	}

	_fontUC.setGlobalDpi(72);
	if (!_fontUC.load(ucPath, cTextCanvasSize, true, true))
	{
		ofLog(OF_LOG_ERROR, "[text2Model::load]Cant load fontUC");
		return;
	}

	_isLoad = true;

}

//--------------------------------------------------------------
vector<ofVboMesh> text2Model::translate(wstring text, float depth)
{
	string word = ws2s(text);
	// contains all of the paths of the current word
	vector <ofPath> word_paths = _fontUC.getStringAsPoints(word, 0);
	vector <ofVboMesh> front_meshes, back_meshes, side_meshes;

	// meshes for the sides and the front of the 3d extruded text
	vector<ofVboMesh> all_meshes; // returned meshese (sides + front + back)

								  // loop through all the characters paths
	for (int i = 0; i < word_paths.size(); i++) {

		ofVboMesh current_char_mesh;

		// 1. create the front mesh using a temporary ofPath and then extract its tessellation

		// for every char break it into polyline
		// (simply a collection of the inner and outer points)
		vector <ofPolyline> char_polylines = word_paths.at(i).getOutline();

		ofVboMesh front; // the final vbos used to store the vertices
		ofPath front_path; // a temp path used for computing the tessellation of the letter shape

						   // now we build an ofPath using the vertices from the character polylines
						   // first loop is for each polyline in the character
						   // see http://openframeworks.cc/documentation/graphics/ofTrueTypeFont/#show_getStringAsPoints
		for (int c = 0; c < char_polylines.size(); c++) {
			// second loop is for each point on the polyline
			for (int p = 0; p < char_polylines[c].size(); p++) {

				if (p == 0) {
					front_path.moveTo(char_polylines[c][p]);
				}
				else {
					front_path.lineTo(char_polylines[c][p]);
				}
			}
		}
		front = front_path.getTessellation();
		ofVec3f * front_vertices = front.getVerticesPointer();

		// compute the back by just offsetting the vertices of the required amount
		ofVboMesh back = front;

		for (int v = 0; v < front.getNumVertices(); v++) {
			front_vertices[v].z += depth;
		}

		current_char_mesh.append(front);
		current_char_mesh.append(back);

		all_meshes.push_back(current_char_mesh);

		// 2. make the extruded sides
		vector <ofPolyline> lines = word_paths.at(i).getOutline();
		for (int j = 0; j < lines.size(); j++) {

			ofVboMesh side;
			vector <ofPoint> points = lines.at(j).getVertices();
			int k = 0;

			for (k = 0; k < points.size() - 1; k++) {
				ofPoint p1 = points.at(k + 0);
				ofPoint p2 = points.at(k + 1);

				side.addVertex(p1);
				side.addVertex(p2);
				side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));
				side.addVertex(ofPoint(p2.x, p2.y, p2.z + depth));
				side.addVertex(p2);
			}

			// connect the last to the first
			ofPoint p1 = points.at(k);
			ofPoint p2 = points.at(0);
			side.addVertex(p1);
			side.addVertex(p2);
			side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));

			side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));
			side.addVertex(ofPoint(p2.x, p2.y, p2.z + depth));
			side.addVertex(p2);

			side.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

			all_meshes.push_back(side);
		}
	}

	return all_meshes;
}

//--------------------------------------------------------------
vector<ofVboMesh> text2Model::translate(string text, float depth)
{
	// contains all of the paths of the current word
	vector <ofPath> word_paths = _font.getStringAsPoints(text, 0);
	vector <ofVboMesh> front_meshes, back_meshes, side_meshes;

	// meshes for the sides and the front of the 3d extruded text
	vector<ofVboMesh> all_meshes; // returned meshese (sides + front + back)

								  // loop through all the characters paths
	for (int i = 0; i < word_paths.size(); i++) {

		ofVboMesh current_char_mesh;

		// 1. create the front mesh using a temporary ofPath and then extract its tessellation

		// for every char break it into polyline
		// (simply a collection of the inner and outer points)
		vector <ofPolyline> char_polylines = word_paths.at(i).getOutline();

		ofVboMesh front; // the final vbos used to store the vertices
		ofPath front_path; // a temp path used for computing the tessellation of the letter shape

						   // now we build an ofPath using the vertices from the character polylines
						   // first loop is for each polyline in the character
						   // see http://openframeworks.cc/documentation/graphics/ofTrueTypeFont/#show_getStringAsPoints
		for (int c = 0; c < char_polylines.size(); c++) {
			// second loop is for each point on the polyline
			for (int p = 0; p < char_polylines[c].size(); p++) {

				if (p == 0) {
					front_path.moveTo(char_polylines[c][p]);
				}
				else {
					front_path.lineTo(char_polylines[c][p]);
				}
			}
		}
		front = front_path.getTessellation();
		ofVec3f * front_vertices = front.getVerticesPointer();

		// compute the back by just offsetting the vertices of the required amount
		ofVboMesh back = front;

		for (int v = 0; v < front.getNumVertices(); v++) {
			front_vertices[v].z += depth;
		}

		current_char_mesh.append(front);
		current_char_mesh.append(back);

		all_meshes.push_back(current_char_mesh);

		// 2. make the extruded sides
		vector <ofPolyline> lines = word_paths.at(i).getOutline();
		for (int j = 0; j < lines.size(); j++) {

			ofVboMesh side;
			vector <ofPoint> points = lines.at(j).getVertices();
			int k = 0;

			for (k = 0; k < points.size() - 1; k++) {
				ofPoint p1 = points.at(k + 0);
				ofPoint p2 = points.at(k + 1);

				side.addVertex(p1);
				side.addVertex(p2);
				side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));
				side.addVertex(ofPoint(p2.x, p2.y, p2.z + depth));
				side.addVertex(p2);
			}

			// connect the last to the first
			ofPoint p1 = points.at(k);
			ofPoint p2 = points.at(0);
			side.addVertex(p1);
			side.addVertex(p2);
			side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));

			side.addVertex(ofPoint(p1.x, p1.y, p1.z + depth));
			side.addVertex(ofPoint(p2.x, p2.y, p2.z + depth));
			side.addVertex(p2);

			side.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

			all_meshes.push_back(side);
		}
	}

	return all_meshes;
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