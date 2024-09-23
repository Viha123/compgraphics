#pragma once

#include "ofMain.h"
#include "glm/gtx/intersect.hpp"
#include "RenderCam.hpp"
#include "Primitives/SceneObject.hpp"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void rayTrace(); // you implement this for the project
		void drawGrid();
		void drawAxis(glm::vec3 position);

    float computeU(int i);
    float computeV(int j);
    
		bool bHide = true;
		bool bShowImage = false;

		ofEasyCam  mainCam;
		ofCamera topCam;
		ofCamera sideCam;
		ofCamera previewCam;
		ofCamera* theCam;    // set to current camera either mainCam or sideCam

		// set up one render camera to render image throughn
		//
		RenderCam renderCam;
		ofImage image;

		vector<SceneObject*> scene;

		int imageWidth = 600;
		int imageHeight = 400;
};