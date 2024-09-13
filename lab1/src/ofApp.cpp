#include "ofApp.h"
#include "intersect.hpp"
#include <limits>

//--------------------------------------------------------------
void ofApp::setup() {
  // set camera parameters
  //
  cam.setDistance(10.0);
  cam.setNearClip(0);
  cam.setTarget(glm::vec3(0, 0, 0));
  ofNoFill();
  verts.push_back(glm::vec3(-1, 0, 1));
  verts.push_back(glm::vec3(1, 0, 1));
  verts.push_back(glm::vec3(1, 0, -1));
  verts.push_back(glm::vec3(-1, 0, -1));
  verts.push_back(glm::vec3(0, 3, 0));
  triangles.push_back({verts[0], verts[1], verts[2]});
  triangles.push_back({verts[0], verts[2], verts[3]});
  triangles.push_back({verts[0], verts[1], verts[4]});
  triangles.push_back({verts[1], verts[2], verts[4]});
  triangles.push_back({verts[2], verts[3], verts[4]});
  triangles.push_back({verts[3], verts[0], verts[4]});
  gui.setup(); // most of the time you don't need a name
  gui.add(beta.setup("beta", .33, 0.0, 1.0));
  gui.add(gamma.setup("gamma", .33, 0.0, 1.0));
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  cam.begin();

  // draw 3D object here  (to-do reverse order)

  // base
  //
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
  if (intersects) {
    ofDrawSphere(intersectionPoint, 0.05);

  }
  ofDrawLine(lineStart, lineEnd);
  cam.end();

  if (!bHide) {
    gui.draw();
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
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
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
  float smallestT = std::round_toward_infinity;

  glm::vec3 origin = cam.screenToWorld(glm::vec3(x, y, 0));
  intersects = false;
  for (auto triangle : triangles) {
    glm::vec2 baryPos;
    float t;
    bool intersect = glm::intersectRayTriangle(
        cam.getPosition(), origin - cam.getPosition(), triangle[0], triangle[1],
        triangle[2], baryPos, t);
    if (intersect) {
      if (t < smallestT) {
        smallestT = t;
        lineStart = cam.getPosition();
		lineEnd = cam.getPosition() + (origin - cam.getPosition()) * t;
		intersectionPoint = cam.getPosition() + (origin - cam.getPosition()) * t;
	  }
      intersects = true;
      
    }
  }
  
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

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