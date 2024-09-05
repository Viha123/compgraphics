#pragma once
#include "ofFileUtils.h"
#include <vector>
#include <array>
class Mesh {

public:
  Mesh(ofFile path);
  void draw();

  std::vector<std::array<float,3>> triangles; // The vertices would be stored in here
  std::vector<int> triangleIndex;  // The index of the triangles that make up a
                                   // mesh are located here

private:
  void populateData();
  ofFile m_file;
};