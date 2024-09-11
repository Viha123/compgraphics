#include "ofApp.h"
#include "intersect.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
	// set camera parameters
	//
	cam.setDistance(10.0);
	cam.setNearClip(.1);
	cam.setTarget(glm::vec3(0, 0, 0));
	ofNoFill();

	gui.setup(); // most of the time you don't need a name
	gui.add(beta.setup("beta", .33, 0.0, 1.0));
	gui.add(gamma.setup("gamma", .33, 0.0, 1.0));

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();

	// draw 3D object here  (to-do reverse order)
	verts.push_back(glm::vec3(-1, 0, 1));
	verts.push_back(glm::vec3(1, 0, 1));
	verts.push_back(glm::vec3(1, 0, -1));
	verts.push_back(glm::vec3(-1, 0, -1));
	verts.push_back(glm::vec3(0, 3, 0));

	// base
	//
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofDrawTriangle(verts[0], verts[2], verts[3]);

	// sides
	//
	ofDrawTriangle(verts[0], verts[1], verts[4]);
	ofDrawTriangle(verts[1], verts[2], verts[4]);
	ofDrawTriangle(verts[2], verts[3], verts[4]);
	ofDrawTriangle(verts[3], verts[0], verts[4]);

	// draw little sphere at a barycentric coordinate
	//
	// get values out of slider
	//
	float b = beta;
	float g = gamma;

	//
	//
	glm::vec3 p =
		verts[0] + b * (verts[1] - verts[0]) + g * (verts[4] - verts[0]);

	ofDrawSphere(p, .1);




	cam.end();

	if (!bHide) {
		gui.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'f':
		ofToggleFullscreen();
		break;
	case 'h':
		bHide = !bHide;
		break;
	default:
		break;

	}

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
void ofApp::mousePressed(int x, int y, int button) {
  glm::vec2 baryPos;
  float t;
  glm::vec3 origin = cam.screenToWorld(glm::vec3(x, y, 0));
  bool intersect =
      glm::intersectRayTriangle(origin, glm::vec3(0, 0, -1),
                                verts[0], verts[1], verts[4], baryPos, t);
  if (intersect) {
    cout << "hit" << endl;
    cout << "t: " << t << endl;
  } else {
    cout << "missed" << endl;
  }
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