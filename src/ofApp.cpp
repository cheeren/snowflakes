#include "ofApp.h"
#include "ofHex.h"
#include <cmath>
#include <queue>

//--------------------------------------------------------------

vector<ofHex> ofApp::buildReflection(vector<ofHex> s,vector<ofVec3f> p){
    
    /*
    Given (x,y) and a line y = ax + c we want the point (x', y') reflected on the line.
    
    Set d:= (x + (y - c)*a)/(1 + a^2)
    
    Then x' = 2*d - x
    
    and y' = 2*d*a - y + 2c */
    
    vector<ofHex> retPolyList;
    
    for (int i = 0; i < s.size(); i++){
        vector<ofVec3f> verts = s[i].getVertices();
        ofHex addPoly;
        for (int j = 0; j < verts.size(); j++){
            //transform each vertex
            double a = -(p[2].x - p[1].x)/(p[2].y - p[1].y);
            double c = -a * p[0].x + p[0].y;
            double d = (verts[j].x + (verts[j].y - c)*a)/(1 + a*a);
            double newX = 2*d - verts[j].x;
            double newY = 2*d*a - verts[j].y + 2*c;
            addPoly.addVertex(newX,newY);
        }
        if (addPoly.plate) addPoly.close();
        retPolyList.push_back(addPoly);
        
    }
    return retPolyList;
}


ofHex ofApp::makeStick(const vector<ofVec3f> & v,double ext,double w){
    // constants we will need:
    double sidelength = v[0].distance(v[1]);
    double r = w/sidelength;
    double xa = (v[1].x + v[2].x)/2;
    double ya = (v[1].y + v[2].y)/2;
    double D = ext + sidelength/2;
    ofHex returnVal;
    
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
    returnVal.addVertex(x3,y3);
    returnVal.plate = false;

    returnVal.close();

    return returnVal;
}

ofHex ofApp::makePlate(const vector<ofVec3f> & v, double ext, double w){
    double sidelength = v[0].distance(v[1]);
    double r = w/sidelength;
    double xa = (v[1].x + v[2].x)/2;
    double ya = (v[1].y + v[2].y)/2;
    double D = ext + sidelength/2;
    ofHex returnVal;
    
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
    returnVal.plate = true;

    returnVal.close();

    return returnVal;
}

ofHex ofApp::makeHex(vector<ofVec3f> pilot) {
    
    double sidelength = pilot[0].distance(pilot[1]);
    double extent = ofRandom(sidelength/2, 2*sidelength);
    double width = ofRandom(3*sidelength/2);
    ofHex returnVal;
    
    if (width > sidelength)
        returnVal = makePlate(pilot, extent, width);
    else
        returnVal = makeStick(pilot, extent, width);
    
    return returnVal;
}

vector<ofHex> ofApp::makeFlake(ofHex current, int depth){
    vector<ofHex> retFlake;
    
    if (depth == 1){
        retFlake.push_back(current);
    }
    else {
        vector<ofVec3f> verts = current.getVertices();
        vector<ofVec3f> pilot1, pilot2;
        pilot1.push_back(verts[2]);
        pilot1.push_back(verts[3]);
        pilot1.push_back(verts[1]);
        ofHex newFlake = makeHex(pilot1);
        retFlake = makeFlake(newFlake,depth-1);
        if (current.plate) {
            pilot2.push_back(verts[1]);
            pilot2.push_back(verts[2]);
            pilot2.push_back(verts[0]);
            ofHex newBranchSeed = makeHex(pilot2);
            vector<ofHex> newBranch = makeFlake(newBranchSeed,depth-1);
            retFlake.insert(retFlake.end(), newBranch.begin(), newBranch.end());
            
            vector<ofHex> reflectBranch = buildReflection(newBranch, pilot1);
            retFlake.insert(retFlake.end(), reflectBranch.begin(), reflectBranch.end());
        }
        retFlake.push_back(current);
    }
    return retFlake;
}

void ofApp::setup(){
    ofHex first;
    for (int i = 0; i < 6; i++)
        first.addVertex(cos(60 * i * pi/180.0),sin(60 * i * pi/180.0));
    first.close();
    
    hexagons = makeFlake(first, 10);
    
    for(int i = 0; i < hexagons.size(); i++) {
        
        //FOR FILLING
        
        ofPath pathFromHex;//path to be built
        
        for(int j = 0; j < 6; j++) {
            if(j == 0) {
                pathFromHex.moveTo(hexagons[i].getVertices()[j]);
            } else {
                pathFromHex.lineTo(hexagons[i].getVertices()[j]);
            }
        }
        pathFromHex.close();
    
        ofColor pathColor(0,0,200,ofRandom(0,127));
        pathFromHex.setFillColor(pathColor);
        paths.push_back(pathFromHex);

        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    //ofSetLineWidth(2.0);  // Line widths apply to polylines
    //ofSetColor(255,100,0);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);  // Translate to the center of the screen
    ofScale(25, 25);
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < hexagons.size(); i++){
            paths[i].draw();
        }
        ofRotate(60);
    }
    ofPopMatrix();
    
 //   ofColor pathColor(ofRandom(255),ofRandom(255),ofRandom(255));
 //   pathFromContour.setFillColor(pathColor);
 //   pathFromContour.draw();
 //   paths.push_back(pathFromContour);

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
