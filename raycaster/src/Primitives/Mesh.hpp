#pragma once

#include "ofMain.h"
#include "glm/gtx/intersect.hpp"
#include "SceneObject.hpp"
//  Mesh class (will complete later- this will be a refinement of Mesh from Project 1)
//
class Mesh : public SceneObject {
	bool intersect(const Ray& ray, glm::vec3& point, glm::vec3& normal) { return false; }
	void draw() { }
};