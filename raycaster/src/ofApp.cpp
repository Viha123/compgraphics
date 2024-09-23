#include "ofApp.h"
#include "Primitives/Sphere.hpp"
#include "ofColor.h"
#include "ofGraphicsConstants.h"
#include "ofPixels.h"
#include <cmath>
#include <cstdlib>
#include <exception>
#include <limits>
// Intersect Ray with Plane  (wrapper on glm::intersect*
//
bool Plane::intersect(const Ray &ray, glm::vec3 &point,
                      glm::vec3 &normalAtIntersect) {
  float dist;
  bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
  if (hit) {
    Ray r = ray;
    point = r.evalPoint(dist);
  }
  return (hit);
}

// Convert (u, v) to (x, y, z)
// We assume u,v is in [0, 1]
//
glm::vec3 ViewPlane::toWorld(float u, float v) {
  float w = width();
  float h = height();
  return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
}

// Get a ray from the current camera position to the (u, v) position on
// the ViewPlane
// convert i, j to uv
Ray RenderCam::getRay(float u, float v) {
  glm::vec3 pointOnPlane = view.toWorld(u, v);
  return (Ray(position, glm::normalize(pointOnPlane - position)));
}

// This could be drawn a lot simpler but I wanted to use the getRay call
// to test it at the corners.
//
void RenderCam::drawFrustum() {
  Ray r1 = getRay(0, 0);
  Ray r2 = getRay(0, 1);
  Ray r3 = getRay(1, 1);
  Ray r4 = getRay(1, 0);
  float dist = glm::length((view.toWorld(0, 0) - position));
  r1.draw(dist);
  r2.draw(dist);
  r3.draw(dist);
  r4.draw(dist);
}

//
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(glm::vec3 position) {

  ofPushMatrix();
  ofTranslate(position);

  ofSetLineWidth(1.0);

  // X Axis
  ofSetColor(ofColor(255, 0, 0));
  ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));

  // Y Axis
  ofSetColor(ofColor(0, 255, 0));
  ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

  // Z Axis
  ofSetColor(ofColor(0, 0, 255));
  ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

  ofPopMatrix();
}

void ofApp::drawGrid() {

  float u = 0;
  float v = 0;
  float pixelWidth = 1.0 / imageWidth;
  float pixelHeight = 1.0 / imageHeight;
  for (int x = 0; x < imageWidth; x++) {
    glm::vec3 p1 = renderCam.view.toWorld(u, 0);
    glm::vec3 p2 = renderCam.view.toWorld(u, 1);
    ofDrawLine(p1, p2);
    u += pixelWidth;
  }
  for (int y = 0; y < imageHeight; y++) {
    glm::vec3 p1 = renderCam.view.toWorld(0, v);
    glm::vec3 p2 = renderCam.view.toWorld(1, v);
    ofDrawLine(p1, p2);
    v += pixelHeight;
  }
}

//--------------------------------------------------------------
void ofApp::setup() {

  ofSetBackgroundColor(ofColor::black);
  ofEnableDepthTest();
  mainCam.setDistance(15);
  mainCam.setNearClip(.1);
  sideCam.setPosition(25, 0, 0);
  sideCam.lookAt(glm::vec3(0, 0, 0));

  topCam.setPosition(0, 25, 0);
  topCam.lookAt(glm::vec3(0, -1, 0));

  previewCam.setPosition(renderCam.position);
  previewCam.setNearClip(.1);
  previewCam.lookAt(glm::vec3(0, 0, 0));
  theCam = &mainCam;

  scene.push_back(new Sphere(glm::vec3(-1, 0, 0), 2.0, ofColor::blue));
  scene.push_back(new Sphere(glm::vec3(1, 0, -4), 2.0, ofColor::green));

  // // ground plane
  // //
  // scene.push_back(new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0),
  // ofColor::brown));
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  if (!bShowImage) {
    theCam->begin();

    ofSetColor(ofColor::green);
    ofNoFill();

    drawAxis(glm::vec3(0, 0, 0));
    ;

    //  draw objects in scene
    //
    // theCam->begin();

    ofSetColor(ofColor::green);
    ofNoFill();

    //  draw objects in scene
    //
    for (int i = 0; i < scene.size(); i++) {
      ofSetColor(scene[i]->diffuseColor);
      scene[i]->draw();
    }

    ofDisableLighting();
    ofSetColor(ofColor::lightSkyBlue);
    renderCam.drawFrustum();
    renderCam.view.draw();
    ofSetColor(ofColor::blue);
    renderCam.draw();
  } else {
    theCam->end();
    image.draw(0, 0);
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

void ofApp::keyReleased(int key) {
  switch (key) {
  case 'C':
  case 'c':
    if (mainCam.getMouseInputEnabled())
      mainCam.disableMouseInput();
    else
      mainCam.enableMouseInput();
    break;
  case 'F':
  case 'b':
    break;
  case 'f':
    ofToggleFullscreen();
    break;
  case 'h':
    bHide = !bHide;
    break;
  case 'i':

    break;
  case 'n':
    scene.push_back(new Sphere(glm::vec3(0, 0, 0), 1.0, ofColor::violet));
    break;
  case 'r':
    if (bShowImage == false) {
      rayTrace();
      bShowImage = true;
    } else {
      bShowImage = false; // toggling the render
    }
    cout << "done..." << endl;
    break;
  case 'm':
    break;
  case OF_KEY_F1:
    theCam = &mainCam;
    break;
  case OF_KEY_F2:
    theCam = &sideCam;
    break;
  case OF_KEY_F3:
    theCam = &previewCam;
    break;
  case OF_KEY_F4:
    theCam = &topCam;
    break;
  default:
    break;
  }
}
float ofApp::computeU(int i) { return (i + 0.5) / imageWidth; }
float ofApp::computeV(int j) { return (j + 0.5) / imageHeight; }
void ofApp::rayTrace() {
  std::cout << "hello im supposed to ray trace here" << std::endl;
  image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);
  ofPixels pixels;
  pixels.allocate(imageWidth, imageHeight, OF_PIXELS_RGB);
  int count = 0;
  for (int i = 0; i < imageWidth; i++) {
    for (int j = 0; j < imageHeight; j++) {

      Ray renderCamRay = renderCam.getRay( computeU(i), computeV(j));
      // std::cout << "\033[32m" << renderCamRay.d << std::endl;
      float closestDistance = std::numeric_limits<float>::max();
      SceneObject *closestShape;
      bool intersects = false;
      for (auto shape : scene) {
        glm::vec3 intersectionNormal;
        glm::vec3 intersectPosition;
        intersects = shape->intersect(renderCamRay, intersectPosition,
                                      intersectionNormal);
        if (intersects) {
          auto distance = std::sqrt((std::pow(intersectPosition.x, 2),
                                     std::pow(intersectPosition.y, 2),
                                     std::pow(intersectPosition.z, 2)));
          if (distance < closestDistance) {
            closestDistance = distance;
            closestShape = shape;
          }
        }
      }
      if (intersects) {
        count += 1;
        pixels[imageWidth * i + j + 0] = closestShape->diffuseColor[0];
        pixels[imageWidth * i + j + 1] = closestShape->diffuseColor[1];
        pixels[imageWidth * i + j + 2] = closestShape->diffuseColor[2];
      } else {
        pixels[imageWidth * i + j + 0] = ofColor::black[0];
        pixels[imageWidth * i + j + 1] = ofColor::black[1];
        pixels[imageWidth * i + j + 2] = ofColor::black[2];
      }
    }
  }
  image.setFromPixels(pixels);
  cout << "\033[32m" << count << "\033[0m" << endl;

}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

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