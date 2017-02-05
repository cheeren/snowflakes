#pragma once

#include "ofMain.h"
#include "ofHex.h"
#include <vector>
using namespace std;

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
    
    vector<ofHex> hexagons;
    vector<ofPath> paths;

    const double pi = 3.1415926535897;
    vector<ofHex> makeFlake(ofHex current, int depth);
    ofHex makeHex(vector<ofVec3f> pilot);
    ofHex makeStick(const vector<ofVec3f> & v, double ex, double w);
    ofHex makePlate(const vector<ofVec3f> & v, double ex, double w);
    vector<ofHex> buildReflection(vector<ofHex> s, vector<ofVec3f> p);

};
