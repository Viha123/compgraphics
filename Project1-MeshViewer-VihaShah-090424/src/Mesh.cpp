#include "Mesh.h"
#include "fwd.hpp"
#include "ofFileUtils.h"
#include "ofGraphics.h"
#include <cassert>
#include <string>
#include "ofGraphics.h"
Mesh::Mesh() {}
void Mesh::loadFile(ofFile file) {
  m_file = file;
  populateData();
  computeNormals();
}

void Mesh::computeNormals() {
  float beta = 0.33;
  float gamma = 0.33;
  
  for (auto triangle : triangleIndex) {
    // std::cout << triangle[0] << " " << triangle[1] << " " << triangle[2] << std::endl;
    auto center = triangles[triangle[0]] + beta * (triangles[triangle[1]] - triangles[triangle[0]]) + gamma * (triangles[triangle[2]]  - triangles[triangle[0]]);
    
    auto cross =
        computeCrossProduct((center - triangles[triangle[1]]),
                            (center - triangles[triangle[0]]));

    // center and cross product
    normalVectors.push_back({center, cross});
    
  }
}
 
void Mesh::populateData() {
  assert(m_file.canRead());
  ofBuffer buffer = m_file.readToBuffer();
  int vertices = 0;
  int faces = 0;
  for (auto line : buffer.getLines()) {
    int i = 0;
    if (line.starts_with("v ")) {
      vertices++;
      line = line.substr(2);
      int idx = line.find(" ");
      std::array<double, 3> vertex;
      while (i < 3) {

        std::string number = line.substr(0, idx);
        // std::cout << number << std::endl;
        
        double value = std::stod(number);
        vertex[i] = value;
        i += 1;
        line = line.substr(idx+1);
        idx = line.find(" ");
      }
      assert(i == 2);
      // if (vertex[0] == 0 || vertex[1] == 0|| vertex[2] == 0) {
      //   std::cout<< line << std::endl;
      // }
      triangles.emplace_back(glm::vec3(vertex[0], vertex[1], vertex[2]));
    }
    if (line.starts_with("f ")) {
      faces++;
      line = line.substr(2);
      int idx = line.find(" ");
      std::array<int, 3> vertexIndex;
      while (i < 3) {

        std::string number = line.substr(0, idx);
        // std::cout << number << std::endl;
        
        int value = std::stoi(number);
        vertexIndex[i] = value -1;
        i += 1;
        line = line.substr(idx+1);
        idx = line.find(" ");
      }
      assert (i == 2);
      triangleIndex.emplace_back(vertexIndex);
      
    }
  }

  std::cout << "Vertices: " << vertices << " Faces: " << faces << " Total KBytes: " << ((vertices * 24) + (faces * 12)) /  1000 << std::endl;
}

void Mesh::draw() {
  for (auto triangle : triangleIndex) {
    ofDrawTriangle(triangles[triangle[0]], triangles[triangle[1]],
                   triangles[triangle[2]]);
    
  }
}

void Mesh::drawNormals(float size) {
  for (auto normal : normalVectors) {
    ofSetColor(255, 100, 0);
    ofDrawLine(normal[0], (size * normal[1] * -1) + normal[0]);
  }
}

glm::vec3 Mesh::computeCrossProduct(glm::vec3 a, glm::vec3 b) {
  // std::cout << a << " " << b << std::endl;
  float x = (a[1] * b[2]) - (a[2]* b[1]);
  float y = -(a[0] * b[2]) + (a[2] * b[0]);
  float z = (a[0] * b[1]) - (a[1] * b[0]);
  // std::cout << x << " " << /y << " " << z << std::endl;
  return glm::vec3(x, y, z);
  
}