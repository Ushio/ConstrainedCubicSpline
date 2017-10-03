#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {
	_camera.setNearClip(0.1f);
	_camera.setFarClip(1000.0f);
	_camera.setDistance(200.0f);

	_imgui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofClear(0);
	ofEnableDepthTest();
	_camera.begin();

	ofPushMatrix();
	ofRotateY(90.0f);
	ofSetColor(64);
	ofDrawGridPlane(100.0f);
	ofPopMatrix();

	ofPushMatrix();
	ofSetLineWidth(3);
	ofDrawAxis(100);
	ofSetLineWidth(1);
	ofPopMatrix();

	static std::vector<ConstrainedCubicSplineInterpolation<float>::DataPoint> pts = {
		{ 0, 30 },{ 10, 130 },{ 50, 150 },{ 70, 170 },{ 90, 220 },{ 100, 320 },
	};
	ConstrainedCubicSplineInterpolation<float> func(pts);

	ofSetColor(255);
	ofNoFill();
	for (int i = 0; i < func.size(); ++i) {
		ofDrawCircle(func[i].x, func[i].y, 2.0);
	}
	ofFill();

	ofPolyline line;
	int N = 500;
	for (int i = 0; i < N; i++) {
		float x = ofMap(i, 0, N - 1, 0, 100);
		float y = func.evaluate(x);
		line.addVertex(x, y);
	}
	line.draw();

	_camera.end();
	ofDisableDepthTest();

	_imgui.begin();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ofVec4f(0.2f, 0.2f, 0.5f, 0.5f));
	ImGui::SetNextWindowPos(ofVec2f(10, 30), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ofVec2f(500, ofGetHeight() * 0.8), ImGuiSetCond_Once);

	ImGui::Begin("Config Panel");


	for (int i = 0; i < pts.size(); ++i) {
		char name[64];
		sprintf(name, "value[%d]", i);
		ImGui::SliderFloat(name, &(pts[i].y), -300, 300);
	}

	auto wp = ImGui::GetWindowPos();
	auto ws = ImGui::GetWindowSize();
	ofRectangle win(wp.x, wp.y, ws.x, ws.y);

	ImGui::End();
	ImGui::PopStyleColor();

	_imgui.end();


	if (win.inside(ofGetMouseX(), ofGetMouseY())) {
		_camera.disableMouseInput();
	}
	else {
		_camera.enableMouseInput();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
