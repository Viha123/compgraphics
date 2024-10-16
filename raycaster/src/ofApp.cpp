#include "ofApp.h"
#include "Primitives/Light.hpp"
#include "Primitives/Mesh.hpp"
#include "Primitives/SceneObject.hpp"
#include "Primitives/Sphere.hpp"
#include "Primitives/ViewPlane.hpp"
#include "fwd.hpp"
#include "geometric.hpp"
#include "ofAppRunner.h"
#include "ofCamera.h"
#include "ofColor.h"
#include "ofGraphics.h"
#include "ofGraphicsConstants.h"
#include "ofPixels.h"
#include "ofxBaseGui.h"
#include "ofxInputField.h"
#include "ofxSlider.h"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <glm/glm.hpp>
#include <limits>
#include <string>

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
  normalAtIntersect = this->normal;
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
  gui.setup();
  gui.setPosition(10, 10); // Set position to top-left corner

  gui.add(diffuseCoeffient.setup("Diffuse Coefficient", 0.5, 0, 1));
  gui.add(spectacularCoefficient.setup("Spectacular Coefficient", 0.05, 0, 1));
  gui.add(phongExponent.setup("Phong Exponent", 140, 10, 10000));

  lighting.setup();
  lighting.setPosition(900, 10);

  lights.push_back(
      new Light(glm::vec3(-4, 2, 2), ofColor::white, lightStartIntensity));

  // lights.push_back(
  //     new Light(glm::vec3(5, 2, 3), ofColor::white, lightStartIntensity));

  lights.push_back(
      new Light(glm::vec3(2, 5, 3), ofColor::white, lightStartIntensity));
  lights.push_back(
      new Light(glm::vec3(3, 0, 1), ofColor::white, lightStartIntensity));

  for (int i = 0; i < lights.size(); i++) {
    ofxIntSlider *lightIntense = new ofxIntSlider();
    std::string name = "Light " + std::to_string(i);
    std::cout << name << std::endl;
    lights[i]->diffuseColor = ofColor::yellow;
    lightIntensity.push_back(
        lightIntense->setup(name, lightStartIntensity, 0, 500));
    lighting.add(lightIntense);
  }

  ofFile file = ofFile();
  file.open("knight_lowpoly.obj", ofFile::ReadOnly);

  mesh.setColor(ofColor::lightSeaGreen);
  mesh.loadFile(file);
  // mesh.setOffset(glm::vec3(0, -3.5, 0)); // place the mesh in a good spot

  scene.push_back(new Sphere(glm::vec3(3, 0, 1), 1.0, ofColor::lightCoral));

  scene.push_back(new Sphere(glm::vec3(-2, 3, 0), 1, ofColor::gold));

  scene.push_back(new Sphere(glm::vec3(-1, 0, 2), 1.5, ofColor::darkOliveGreen));

  scene.push_back(&mesh);

  // // ground plane
  // //
  ground = new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0),
                     ofColor::darkOrchid);
  ground->toggleDraggable();

  backgroundPlane =
      new Plane(glm::vec3(0, 0, -15), glm::vec3(0, 0, 1), ofColor::lightYellow);
  backgroundPlane->toggleDraggable();
  scene.push_back(ground);
  scene.push_back(backgroundPlane);
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {

  theCam->begin();

  ofSetColor(ofColor::green);
  ofNoFill();

  drawAxis(glm::vec3(0, 0, 0));

  //  draw objects in scene
  //
  for (size_t i = 0; i < scene.size(); i++) {
    if (scene[i]->isSelected) {
      // scene[i]->print();
      ofSetColor(255, 100, 0);
      scene[i]->draw();

    } else {
      ofSetColor(scene[i]->diffuseColor);

      scene[i]->draw();
      // scene[i]->print();
      // std::cout << scene[i]->diffuseColor << std::endl;
    }
    // std::cout << scene[i]->diffuseColor << std::endl;
  }

  // draw lights
  for (size_t i = 0; i < lights.size(); i++) {
    if (lights[i]->isSelected) {
      ofSetColor(255, 100, 0);

    } else {
      ofSetColor(ofColor::white);
    }
    lights[i]->draw();
    lights[i]->setIntensity(*lightIntensity[i]);
  }

  ofDisableLighting();
  // ofSetColor(ofColor::lightSkyBlue);
  renderCam.drawFrustum();
  renderCam.view.draw();
  // ofSetColor(ofColor::blue);
  renderCam.draw();

  if (bShowImage) {

    glm::vec3 topLeft = renderCam.view.toWorld(0, 0);
    glm::vec3 topRight = renderCam.view.toWorld(1, 0);
    glm::vec3 bottomLeft = renderCam.view.toWorld(0, 1);

    float worldWidth = glm::distance(topLeft, topRight);
    float worldHeight = glm::distance(topLeft, bottomLeft);
    ofScale(worldWidth / image.getWidth(), worldHeight / image.getHeight());
    image.draw(imageWidth / -2.0, imageHeight / -2.0,
               renderCam.view.position.z);
  }
  if (!bHide) {
    mesh.drawNormals(1);
  }
  // std::cout << normalPlane.normal << std::endl;
  theCam->end();

  ofDisableDepthTest();

  gui.draw();
  lighting.draw();
  ofEnableDepthTest();
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
    // dynamic_cast<Mesh*>(highlightedShape)->drawNormals(10);
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
float ofApp::computeU(int j) { return (j + 0.5) / imageWidth; }
float ofApp::computeV(int i) { return (i + 0.5) / imageHeight; }
void ofApp::rayTrace() {
  image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);
  std::ofstream outFile("meshShaders.txt");
  outFile.clear();
  int count = 0;
  for (int i = 0; i < imageHeight; i++) {
    for (int j = 0; j < imageWidth; j++) {

      Ray renderCamRay = renderCam.getRay(
          computeU(j),
          computeV(imageHeight - i -
                   1)); // becupase the i is the rows which means y axis
      // std::cout << "\033[32m" << renderCamRay.d << std::endl;
      float closestDistance = std::numeric_limits<float>::max();
      SceneObject *closestShape = nullptr;
      bool intersects = false;
      glm::vec3 nearestIntersectPos;
      glm::vec3 nearestIntersectNorm;
      for (auto shape : scene) {
        glm::vec3 intersectionNormal;
        glm::vec3 intersectPosition;
        intersects = shape->intersect(renderCamRay, intersectPosition,
                                      intersectionNormal);
        if (intersects) {
          float distance =
              abs(glm::distance(intersectPosition, renderCam.position));

          if (distance < closestDistance) {
            closestDistance = distance;
            closestShape = shape;
            nearestIntersectPos = intersectPosition;
            nearestIntersectNorm = intersectionNormal;
          }
        }
      }
      if (closestShape != nullptr) {
        // Implementing this equation: c = cr (ca + cl max (0, n · l)) + cl (h ·
        // n)p .
        // count += 1;
        ofColor color = lambert_phong(nearestIntersectPos, nearestIntersectNorm,
                                      closestShape->diffuseColor, ofColor::pink,
                                      phongExponent, count);
        // ofColor color = lambert(nearestIntersectPos, nearestIntersectNorm,
        // closestShape->diffuseColor);
        if (dynamic_cast<Mesh *>(closestShape) != nullptr) {
          // mesh
          // std::cout << closestShape->diffuseColor << " " << color <<
          // std::endl;
          // std::cout << nearestIntersectNorm << " " << color << std::endl;
          outFile << "Normal: (" << nearestIntersectNorm.x << ", "
                  << nearestIntersectNorm.y << ", " << nearestIntersectNorm.z
                  << ")\n";
        }
        image.setColor(j, i, color);
      } else {

        image.setColor(j, i, ofColor::black);
      }
    }
  }
  outFile.close();
  image.update();
  image.save("raytrace.png");
  cout << "\033[32m" << count << "\033[0m" << endl;
}
ofColor ofApp::lambert(const glm::vec3 &p, const glm::vec3 &norm,
                       const ofColor diffuse) {

  // p is the point of intersection
  // norm is the normal vector
  // loop through all lights
  // make vector from position to light
  float totalDiffusedLight = 0;
  for (auto light : lights) {
    // calculate vector and normalize it between light and intersection
    // normalize the normal vector
    // Ld = kd(I/r^2)max(0, n dot l)
    // multiply diffuseColor with Ld and return that
    float diffuseCoeffient = 1;
    glm::vec3 lightVec = glm::normalize(light->position - p);
    glm::vec3 normalizedN = glm::normalize(norm);
    float r = glm::distance(light->position, p);
    float diffusedLight =
        diffuseCoeffient * (light->intensity / glm::pow(r, 2)) *
        glm::max(0.0f, static_cast<float>(glm::dot(lightVec, normalizedN)));
    totalDiffusedLight += diffusedLight;
  }
  ofColor diffusedColor = totalDiffusedLight * diffuse;
  return diffusedColor;
}
ofColor ofApp::lambert_phong(const glm::vec3 &p, const glm::vec3 &norm,
                             const ofColor diffuse, const ofColor spectacular,
                             float power, int &count) {
  // spectacular is the color of your highlights
  // power is the power exponent between 10 and 10000
  float totalPhong = 0;
  float totalLambert = 0;
  float ambient = 0.25;
  SceneObject *closest = nullptr;
  for (auto light : lights) {
    glm::vec3 offset = glm::vec3(0.1, 0.1, 0.1);
    Ray shadowRay(p + offset, glm::normalize(light->position - (p + offset)));
    float distanceToLight = glm::distance(p, light->position);
    bool inShadow = false;
    for (auto object : scene) {
      glm::vec3 intersectPoint;
      glm::vec3 intersectNormal;
      if (object->intersect(shadowRay, intersectPoint, intersectNormal)) {

        float distanceToIntersection = glm::distance(p, intersectPoint);
        if (distanceToIntersection < distanceToLight) {
          inShadow = true;
          break;

        }
      }
    }
    if (!inShadow && dynamic_cast<Mesh *>(closest) != nullptr) {
      count++; // Increment count only if the closest object is a Mesh and not
               // in shadow
    }

    if (!inShadow) {

      glm::vec3 lightVec = glm::normalize(light->position - p);
      glm::vec3 normalizedN = glm::normalize(norm);
      glm::vec3 viewVec = glm::normalize(renderCam.position - p); // View vector

      glm::vec3 bisector = glm::normalize(viewVec + lightVec);
      float r = glm::distance(light->position, p);
      float spectacularLight =
          spectacularCoefficient * (light->intensity / glm::pow(r, 2)) *
          glm::pow(glm::max(0.0f, static_cast<float>(
                                      glm::dot(bisector, normalizedN))),
                   power);
      float diffusedLight =
          diffuseCoeffient * (light->intensity / glm::pow(r, 2)) *
          glm::max(0.0f, static_cast<float>(glm::dot(lightVec, normalizedN)));
      totalPhong += spectacularLight;
      totalLambert += diffusedLight;
    }
  }
  return (totalPhong * spectacular) + ((totalLambert + ambient) * diffuse);
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
  // drag objects in scence
  // drag objects in 3D
  // drag in the plane of the camera
  if (highlightedShape != nullptr and highlightedShape->isSelected == true) {
    // disable camera and do dragging.
    // highlightedShape->print();
    draggingOn = true;

    mainCam.disableMouseInput();
    glm::vec3 worldPoint = theCam->screenToWorld(glm::vec3(x, y, 0));

    normalPlane.position = shapeIntersectionPoint;
    normalPlane.normal = theCam->getZAxis();

    // std::cout << normalPlane.normal << std::endl;
    // std::cout << normal << std::endl;
    Ray ray(theCam->getPosition(),
            glm::normalize(worldPoint - theCam->getPosition()));
    glm::vec3 intersectionPoint;
    [[maybe_unused]] glm::vec3 normalVec; // unused
    if (normalPlane.intersect(ray, intersectionPoint, normalVec)) {
      Mesh *temp = dynamic_cast<Mesh *>(highlightedShape);
      if (temp == nullptr) {

        highlightedShape->position =
            intersectionPoint - offsetIntersectionPoint;
      } else {
        temp->setOffset(intersectionPoint - shapeIntersectionPoint);
      }
    }
  }
  if (highlightedShape == nullptr or highlightedShape->isSelected == false) {
    mainCam.enableMouseInput();
  }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  glm::vec3 worldPoint = theCam->screenToWorld(glm::vec3(x, y, 0));
  float closestDistance = std::numeric_limits<float>::max();
  highlightedShape = nullptr;
  auto processElement = [&](auto element) {
    if (element->isDraggable) {
      Ray ray(worldPoint, glm::normalize(worldPoint - theCam->getPosition()));
      glm::vec3 p, n;
      if (element->intersect(ray, p, n)) {
        float distance = glm::distance(p, worldPoint);
        if (distance < closestDistance) {
          closestDistance = distance;
          highlightedShape = element;
          offsetIntersectionPoint = p - element->position;
          shapeIntersectionPoint = p;
        }
        // std::cout << "intersected";
        // element->print();
      }
      element->isSelected = false; // regardless just make it
    }
  };
  for (auto element : scene) {

    processElement(element);
  }
  for (auto light : lights) {
    processElement(light);
  }

  if (highlightedShape != nullptr) {
    highlightedShape->isSelected = true;
    // highlightedShape->print();
    // cout intersects pointd
    // else
    // cout << "no intersection" << endl;
  }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
  draggingOn = false;
  theCam = &mainCam;
  if (highlightedShape != nullptr) {
    highlightedShape->isSelected = false;
    // offsetIntersectionPoint = glm::vec3(0,0,0);
  }
}

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

// glm::normalize function
//