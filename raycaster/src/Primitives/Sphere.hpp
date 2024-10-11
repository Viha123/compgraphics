#pragma once

#include "SceneObject.hpp"
#include "glm/gtx/intersect.hpp"
#include "ofColor.h"
#include "ofMain.h"
//  General purpose sphere  (assume parametric)
//
class Sphere : public SceneObject {
public:
  Sphere(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) {
    position = p;
    radius = r;
    diffuseColor = diffuse;
  }
  Sphere() {}
  bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
    return (
        glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
  }
  void draw() { ofDrawSphere(position, radius); }
  void print() { std::cout << "Sphere " << diffuseColor << std::endl;}
  void setColor(ofColor color) {diffuseColor = color;}
  float radius = 1.0;
};
