#pragma once

#include "Primitives/Sphere.hpp"
#include "SceneObject.hpp"
#include "of3dGraphics.h"
#include "ofColor.h"
class Light : public SceneObject {
public:
  Light(glm::vec3 p, float i) {
    position = p;
    intensity = i;
    
    sphere = new Sphere(p, 0.3, ofColor::white);
  }
  Light() {};
  Light(float i) {
    intensity = i;
    sphere = new Sphere(position, 0.3, ofColor::white);
  }
  Light(glm::vec3 p) {
    position = p;
    sphere = new Sphere(p, 0.3, ofColor::white);
  }

  void draw() { sphere->draw(); }
  bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
    return (
        glm::intersectRaySphere(ray.p, ray.d, position, 0.3, point, normal));
  }
  void setIntensity(float newIntensity) {
    intensity = newIntensity;
  }
  void print() { std::cout << "Light" << std::endl; }
  float intensity = 10;
  glm::vec3 position = glm::vec3(0, 10, 0); // on top initially
  void setColor(ofColor newColor) {diffuseColor = newColor;}
  
  Sphere *sphere = new Sphere(position, 0.3, ofColor::white);
  
};