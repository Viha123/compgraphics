#pragma once

#include "SceneObject.hpp"
#include "glm/gtx/intersect.hpp"
#include "ofMain.h"
//  Mesh class (will complete later- this will be a refinement of Mesh from
//  Project 1)
//
class Mesh : public SceneObject {
public:
  Mesh();
  bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
    return false;
  }
  void draw();
  void print() { std::cout << "Mesh" << std::endl; }
  void drawNormals(float size);
  void loadFile(ofFile file);
  void computeNormals();
  std::vector<glm::vec3> triangles; // The vertices would be stored in here
  std::vector<std::array<int, 3>>
      triangleIndex; // The index of the triangles that make up a
                     // mesh are located here
  std::vector<std::array<glm::vec3, 2>> normalVectors;
  glm::vec3 computeCrossProduct(glm::vec3 a, glm::vec3 b);
private:
  void populateData();
  ofFile m_file;
};