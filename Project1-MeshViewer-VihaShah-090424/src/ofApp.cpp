#include "ofApp.h"
#include "Mesh.h"
#include "of3dUtils.h"
#include "ofAppRunner.h"
#include "ofFileUtils.h"
#include "ofxBaseGui.h"

//--------------------------------------------------------------
void ofApp::setup() {
  ofFile file = ofFile();
  file.open("chessTriangle.obj", ofFile::ReadOnly);
  cam.setDistance(10.0);
  cam.setNearClip(0.1);
  ofNoFill();
  mesh.loadFile(file); // initialzies the mesh and populates the
  gui.setup();
  gui.add(size.setup("Size Factor", 5, 1, 100));
  

}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  cam.begin();
  ofSetColor(0, 0, 0);
  // ofDrawGrid(1);
  mesh.draw();
  if (!normalsHide) {
    mesh.drawNormals(size);
  }
  cam.end();
  gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  switch (key) {
    case 'f':
      ofToggleFullscreen();
      break;
    case 'h':
      normalsHide = !normalsHide;
      break;
  };
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
