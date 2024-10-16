#pragma once
#include "ofMain.h"
#include "ofxPanel.h"
#include "ofxSlider.h"
#include <glm/gtx/intersect.hpp>
class ofApp : public ofBaseApp {
public:
  void setup();
  void update();
  void draw();
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  ofEasyCam cam;
  bool bHide = false;
  ofxFloatSlider beta, gamma;
  ofxPanel gui;
	vector<glm::vec3> verts;
	vector<std::array<glm::vec3,3>> triangles;
  glm::vec3 intersectionPoint;
  glm::vec3 lineStart;
  glm::vec3 lineEnd;
  bool intersects;
        
};
