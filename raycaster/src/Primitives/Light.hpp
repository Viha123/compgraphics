#pragma once

#include "SceneObject.hpp"
#include "glm/gtx/intersect.hpp"
#include "ofColor.h"
#include "ofMain.h"
//  General purpose sphere  (assume parametric)
//
class Light : public SceneObject {
public:
  Light(glm::vec3 p, ofColor diffuse = ofColor::white, int i = 5) {
    position = p;
    diffuseColor = diffuse;
    intensity = i;
  }
  Light() {}
  bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
    return (
        glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
  }
  void draw() { ofDrawSphere(position, radius); }
  void print() { std::cout << "Light " << diffuseColor << std::endl;}
  void setColor(ofColor color) {diffuseColor = color;}
  float radius = 0.3;
  int intensity = 5;
  void setIntensity(int newIntensity) {intensity = newIntensity;}
};
