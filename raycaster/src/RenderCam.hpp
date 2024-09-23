#pragma once

#include "ofMain.h"
#include "glm/gtx/intersect.hpp"

#include "Primitives/SceneObject.hpp"
#include "Primitives/Ray.hpp"
#include "Primitives/ViewPlane.hpp"

//  render camera  - currently must be z axis aligned (we will improve this in project 4)
//
class RenderCam : public SceneObject {
public:
	RenderCam() {
		position = glm::vec3(0, 0, 10);
		aim = glm::vec3(0, 0, -1);
	}
	Ray getRay(float u, float v);
	void draw() { ofDrawBox(position, 1.0); };
	void drawFrustum();

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 
};
