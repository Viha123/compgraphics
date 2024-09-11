#pragma once
#include "ofFileUtils.h"
#include <vector>
#include <array>
#include "ofxGui.h"
class Mesh {

public:
  Mesh();
  void draw();
  void drawNormals(float beta, float gamma);
  void loadFile(ofFile file);
  std::vector<glm::vec3> triangles; // The vertices would be stored in here
  std::vector<std::array<int,3>> triangleIndex;  // The index of the triangles that make up a
                                   // mesh are located here
  glm::vec3 computeCrossProduct(glm::vec3 a, glm::vec3 b);
  
private:
  void populateData();
  ofFile m_file;
};