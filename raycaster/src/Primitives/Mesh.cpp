#include "Primitives/Mesh.hpp"
#include "Primitives/Ray.hpp"
#include "fwd.hpp"
#include "geometric.hpp"
#include "intersect.hpp"
#include "ofFileUtils.h"
#include "ofGraphics.h"
#include <cassert>
#include <string>
#include <system_error>
Mesh::Mesh() {}
Mesh::Mesh(ofColor color) { diffuseColor = color; };
void Mesh::setColor(ofColor color)  { diffuseColor = color; };
void Mesh::loadFile(ofFile file) {
  m_file = file;
  populateData();
  computeNormals();
}

void Mesh::computeNormals() {
  float beta = 0.33;
  float gamma = 0.33;

  for (auto triangle : triangleIndex) {
    // std::cout << triangle[0] << " " << triangle[1] << " " << triangle[2] <<
    // std::endl;
    auto center = triangles[triangle[0]] +
                  beta * (triangles[triangle[1]] - triangles[triangle[0]]) +
                  gamma * (triangles[triangle[2]] - triangles[triangle[0]]);

    auto cross = computeCrossProduct((center - triangles[triangle[1]]),
                                     (center - triangles[triangle[0]]));

    // center and cross product
    normalVectors.push_back({center, cross});
  }
}
bool Mesh::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
  bool intersectsObject = false;

  float smallestT = std::numeric_limits<float>::infinity();
  int i = 0;
  for (auto triangle : triangleIndex) {
    glm::vec2 baryPos;
    float t;
    bool intersect = glm::intersectRayTriangle(
        ray.p, ray.d, triangles[triangle[0]], triangles[triangle[1]],
        triangles[triangle[2]], baryPos, t);
    if (intersect) {
      if (t < smallestT) {
        smallestT = t;
        point = ray.p + (ray.d) * t;
        normal = glm::normalize(normalVectors[i][1]*-1);
      }
      intersectsObject = true;
    }
    i ++;
  }
  
  return intersectsObject;
}
void Mesh::setOffset(glm::vec3 offset) {
  m_offset = offset;
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
        line = line.substr(idx + 1);
        idx = line.find(" ");
      }
      assert(i == 2);
      // if (vertex[0] == 0 || vertex[1] == 0|| vertex[2] == 0) {
      //   std::cout<< line << std::endl;
      // }
      triangles.emplace_back(glm::vec3(vertex[0] + m_offset[0], vertex[1] + m_offset[1], vertex[2] + m_offset[2]));
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
        vertexIndex[i] = value - 1;
        i += 1;
        line = line.substr(idx + 1);
        idx = line.find(" ");
      }
      assert(i == 2);
      triangleIndex.emplace_back(vertexIndex);
    }
  }

  std::cout << "Vertices: " << vertices << " Faces: " << faces
            << " Total KBytes: " << ((vertices * 24) + (faces * 12)) / 1000
            << std::endl;
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
  float x = (a[1] * b[2]) - (a[2] * b[1]);
  float y = -(a[0] * b[2]) + (a[2] * b[0]);
  float z = (a[0] * b[1]) - (a[1] * b[0]);
  // std::cout << x << " " << /y << " " << z << std::endl;
  return glm::vec3(x, y, z);
}