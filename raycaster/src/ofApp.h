#pragma once

#include "Primitives/Light.hpp"
#include "Primitives/SceneObject.hpp"
#include "RenderCam.hpp"
#include "fwd.hpp"
#include "glm/gtx/intersect.hpp"
#include "ofColor.h"
#include "ofMain.h"
#include "ofxSlider.h"
#include "ofxPanel.h"
#include "ofxButton.h"
#include "ofxGui.h"
#include "Primitives/Mesh.hpp"

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

  void rayTrace(); // you implement this for the project
  void drawGrid();
  void drawAxis(glm::vec3 position);
  ofColor lambert(const glm::vec3 &p, const glm::vec3 &norm,
                  const ofColor diffuse);
  ofColor lambert_phong(const glm::vec3 &p, const glm::vec3 &norm,
                        const ofColor diffuse, const ofColor spectacular,
                        float power);

  float computeU(int i);
  float computeV(int j);

  bool bHide = true;
  bool bShowImage = false;
  // Sliders
  ofxFloatSlider diffuseCoeffient;
  ofxFloatSlider spectacularCoefficient;
  ofxIntSlider phongExponent;
	ofxPanel gui;
  ofEasyCam mainCam;
  ofCamera topCam;
  ofCamera sideCam;
  ofCamera previewCam;
  ofCamera *theCam; // set to current camera either mainCam or sideCam

  // set up one render camera to render image throughn
  //
  RenderCam renderCam;
  ofImage image;

  Mesh mesh;
  vector<SceneObject *> scene;
  vector<Light *> lights;
  int imageWidth = 600;
  int imageHeight = 400;
};