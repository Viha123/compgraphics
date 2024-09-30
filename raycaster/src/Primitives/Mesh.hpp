#pragma once

#include "SceneObject.hpp"
#include "intersect.hpp"
#include "ofColor.h"
#include "ofMain.h"
//  Mesh class (will complete later- this will be a refinement of Mesh from
//  Project 1)
//
class Mesh : public SceneObject {
public:
  Mesh();
  Mesh(ofColor c);
  bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
  void draw();
  void print() { std::cout << "Mesh" << std::endl; }
  void drawNormals(float size);
  void loadFile(ofFile file);
  void computeNormals();
  void setColor(ofColor color);
  std::vector<glm::vec3> triangles; // The vertices would be stored in here
  // std::vector<std::array<glm::vec3,3>> triangles;

  std::vector<std::array<int, 3>>
      triangleIndex; // The index of the triangles that make up a
                     // mesh are located here
  std::vector<std::array<glm::vec3, 2>> normalVectors;
  glm::vec3 computeCrossProduct(glm::vec3 a, glm::vec3 b);
  ofColor diffuseColor;
private:
  void populateData();
  ofFile m_file;
};