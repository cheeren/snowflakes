#include "ofApp.h"
#include <cmath>
#include <queue>

//--------------------------------------------------------------

ofPolyline ofApp::makeStick(const vector<ofVec3f> & v,double ext,double w){
    // constants we will need:
    double sidelength = v[0].distance(v[1]);
    double r = w/sidelength;
    double xa = (v[1].x + v[2].x)/2;
    double ya = (v[1].y + v[2].y)/2;
    double D = ext + sidelength/2;
    ofPolyline returnVal;
    
    // make vertex 0:
    double x0 = 2 * (D * v[0].x - ext * xa)/sidelength;
    double y0 = 2 * (D * v[0].y - ext * ya)/sidelength;
    
    // make vertex 1:
    double x1 = ((2*D - w)*v[0].x - 2*ext*xa + w*v[1].x)/sidelength;
    double y1 = ((2*D - w)*v[0].y - 2*ext*ya + w*v[1].y)/sidelength;
    
    // make vertex 2:
    double x2 = (1-r) * v[0].x + r * v[1].x;
    double y2 = (1-r) * v[0].y + r * v[1].y;
    
    // make vertex 3:
    double x3 = r * (v[1].x + v[2].x) + (1 - (2*r)) * v[0].x;
    double y3 = r * (v[1].y + v[2].y) + (1 - (2*r)) * v[0].y;
    //returnVal.addVertex(x3,y3);
    
    // make vertex 4:
    double x4 = (1-r) * v[0].x + r * v[2].x;
    double y4 = (1-r) * v[0].y + r * v[2].y;
    
    // make vertex 5:
    double x5 = ((2*D - w)*v[0].x - 2*ext*xa + w*v[2].x)/sidelength;
    double y5 = ((2*D - w)*v[0].y - 2*ext*ya + w*v[2].y)/sidelength;
    
    returnVal.addVertex(x4,y4);
    returnVal.addVertex(x5,y5);
    returnVal.addVertex(x0,y0);
    returnVal.addVertex(x1,y1);
    returnVal.addVertex(x2,y2);

    return returnVal;
}

ofPolyline ofApp::makePlate(const vector<ofVec3f> & v, double ext, double w){
    double sidelength = v[0].distance(v[1]);
    double r = w/sidelength;
    double xa = (v[1].x + v[2].x)/2;
    double ya = (v[1].y + v[2].y)/2;
    double D = ext + sidelength/2;
    ofPolyline returnVal;
    
    // make vertex 0:
    double x0 = 2 * (D * v[0].x - ext * xa)/sidelength;
    double y0 = 2 * (D * v[0].y - ext * ya)/sidelength;
    
    // make vertex 1:
    double xtemp = (1-r) * v[0].x + r * v[1].x;
    double ytemp = (1-r) * v[0].y + r * v[1].y;
    double x1 = x0 - v[0].x + xtemp;
    double y1 = y0 - v[0].y + ytemp;
    
    // make vertex 3:
    double x3 = 2*w*v[0].x/ext - (2*w-ext)*x0/ext;
    double y3 = 2*w*v[0].y/ext - (2*w-ext)*y0/ext;
    
    // make vertex 5:
    xtemp = (1-r) * v[0].x + r * v[2].x;
    ytemp = (1-r) * v[0].y + r * v[2].y;
    double x5 = x0 - v[0].x + xtemp;
    double y5 = y0 - v[0].y + ytemp;
    
    // make vertex 2:
    double x2 = 2*x1 - 2*x0+ x5;
    double y2 = 2*y1 - 2*y0+ y5;

    // make vertex 4:
    double x4 = 2*x5 - 2*x0 + x1;
    double y4 = 2*y5 - 2*y0 + y1;
    
    returnVal.addVertex(x4,y4);
    returnVal.addVertex(x5,y5);
    returnVal.addVertex(x0,y0);
    returnVal.addVertex(x1,y1);
    returnVal.addVertex(x2,y2);
    returnVal.addVertex(x3,y3);

    returnVal.close();

    return returnVal;
}

ofPolyline ofApp::makeNextHex(vector<ofVec3f> pilot) {
    
    double sidelength = pilot[0].distance(pilot[1]);
    double extent = ofRandom(sidelength/2, 2*sidelength);
    double width = ofRandom(3*sidelength/2);
    ofPolyline returnVal;
    
    if (width > sidelength)
        returnVal = makePlate(pilot, extent, width);
    else
        returnVal = makeStick(pilot, extent, width);
    
    return returnVal;
}

void ofApp::setup(){
    ofPolyline first;
    for (int i = 0; i < 6; i++)
        first.addVertex(cos(60 * i * pi/180.0),sin(60 * i * pi/180.0));
    first.close();
    
    queue<ofPolyline> Q;
    
    Q.push(first);
    hexagons.push_back(first);

    while (Q.size() > 0 && Q.size() < 20) {
        ofPolyline current = Q.front(); Q.pop();
        vector<ofVec3f> verts = current.getVertices();
        vector<ofVec3f> pilot;
        pilot.push_back(verts[2]);
        pilot.push_back(verts[3]);
        pilot.push_back(verts[1]);
        ofPolyline newStem = makeNextHex(pilot);
        Q.push(newStem);
        hexagons.push_back(newStem);


        if (verts.size() == 6) {

            pilot[0] = verts[1];
            pilot[1] = verts[2];
            pilot[2] = verts[0];
            ofPolyline newBranch = makeNextHex(pilot);
            Q.push(newBranch);
            hexagons.push_back(newBranch);

        }
            
    }
    
/*
    
    curvedSegmentPolyline.curveTo(350, 100);  // These curves are Catmull-Rom splines
    curvedSegmentPolyline.curveTo(350, 100);  // Necessary Duplicate for Control Point
    curvedSegmentPolyline.curveTo(400, 150);
    curvedSegmentPolyline.curveTo(450, 100);
    curvedSegmentPolyline.curveTo(500, 150);
    curvedSegmentPolyline.curveTo(550, 100);
    curvedSegmentPolyline.curveTo(550, 100);  // Necessary Duplicate for Control Point
    
    closedShapePolyline.addVertex(600, 125);
    closedShapePolyline.addVertex(700, 100);
    closedShapePolyline.addVertex(800, 125);
    closedShapePolyline.addVertex(700, 150);
    closedShapePolyline.close();*/
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetLineWidth(2.0);  // Line widths apply to polylines
    ofSetColor(255,100,0);
    /*
    straightSegmentPolyline.draw();// This is how we draw polylines
    vector<ofVec3f> vertices1 = straightSegmentPolyline.getVertices();
    for (int vertexIndex=0; vertexIndex<vertices1.size(); vertexIndex++) {
        ofVec3f vertex = vertices1[vertexIndex];  // ofVec3f is like ofVec2f, but with a third dimension, z
        ofDrawCircle(vertex, 5);
    }
    curvedSegmentPolyline.draw();  // Nice and easy, right?
    vector<ofVec3f> vertices2 = curvedSegmentPolyline.getVertices();
    for (int vertexIndex=0; vertexIndex<vertices2.size(); vertexIndex++) {
        ofVec3f vertex = vertices2[vertexIndex];  // ofVec3f is like ofVec2f, but with a third dimension, z
        ofDrawCircle(vertex, 5);
    }
    closedShapePolyline.draw();
    vector<ofVec3f> vertices3 = closedShapePolyline.getVertices();
    for (int vertexIndex=0; vertexIndex<vertices3.size(); vertexIndex++) {
        ofVec3f vertex = vertices3[vertexIndex];  // ofVec3f is like ofVec2f, but with a third dimension, z
        ofDrawCircle(vertex, 5);
    } */
    
    ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);  // Translate to the center of the screen
        ofScale(25, 25);
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 20; i++)
            hexagons[i].draw();
    ofRotate(120);
    }

    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
