#include "Mesh.h"
#include <cassert>
#include <string>

Mesh::Mesh(ofFile file) {
  m_file = file;
  populateData();
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
      std::array<float, 3> vertex;
      while (i < 3) {

        std::string number = line.substr(0, idx);
        std::cout << number << std::endl;
        
        float value = std::stof(number);
        vertex[i] = value;
        i += 1;
        line = line.substr(idx+1);
        idx = line.find(" ");
      }
      assert(i == 2);
      triangles.emplace_back(vertex);
      std::cout << "saw vertex: " << vertex[0] << " "<< vertex[1] << " " << vertex[2] << " "<< std::endl;
    }
    if (line.starts_with("f ")) {
      faces++;
      std::cout << "saw face" << std::endl;
      
    }
  }

  std::cout << vertices << "  faces: " << faces << std::endl;

}