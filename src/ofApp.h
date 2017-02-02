#pragma once

#include "ofMain.h"
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
    
    ofPolyline straightSegmentPolyline;
    ofPolyline curvedSegmentPolyline;
    ofPolyline closedShapePolyline;
    vector<ofPolyline> hexagons;
    const double pi = 3.1415926535897;
    ofPolyline makeNextHex(ofPolyline h);
    ofPolyline makeStick(const vector<ofVec3f> & v, double ex, double w);
    ofPolyline makePlate(const vector<ofVec3f> & v, double ex, double w);

};
