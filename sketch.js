// Hex Snowflake
// Cinda Heeren
// 12/10/2020

let saveButton;
let timer = 0;
let f;

function setup() {
  createCanvas(600, 600);
  angleMode(DEGREES);
  background(255, 255, 255); // white
  saveButton = createButton('save');
  saveButton.mousePressed(saveSnowflake);
  
  f = new flake();
}

function saveSnowflake() {
  save('snowflake.png');
}

function draw() {
  timer++;
  clear();
  f.display(timer);   
}

class flake {
    constructor(){
        this.translatex = 0.5;
        this.translatey = 0.5;
        this.rotspeed = 2*random() - 1;
        this.growthrate = random();
        this.scale = 20;
        let pts = [];
        for (let i = 0; i < 6; i++){
            pts.push({x: this.scale*cos(60*i), y: this.scale*sin(60*i)});
        }
        let h = new sixgon(pts,false,0);
        this.hexes = this.makeFlake(h,10);
    }

    display(timer){
        translate(width * this.translatex, height * this.translatey);
        //rotate(timer*this.rotspeed);
        for (let i = 0; i < min(this.hexes.length,timer/2); i++) {
            for (let j = 0; j < 6; j++){
                this.hexes[i].display();
                rotate(60);
            }
        }  
    }

    buildReflection(s, p){
        let retHexList = [];
        for (let i = 0; i < s.length; i++){
          let verts = s[i].vertices;
          let h = [];
          for (let j = 0; j < 6; j++){
            let a = -(p[2].x - p[1].x)/(p[2].y - p[1].y);
            let c = -a * p[0].x + p[0].y;
            let d = (verts[j].x + (verts[j].y - c)*a)/(1 + a*a);
            let newX = 2*d - verts[j].x;
            let newY = 2*d*a - verts[j].y + 2*c;
            h.push({x:newX,y:newY}); 
          }
          let newHex = new sixgon(h, s[i].plate,s[i].level);
          newHex.setColor(s[i].col);
          retHexList.push(newHex);
        }
        return retHexList;
      }
      
    makeStick(v, ext, w, d){
          // constants we will need:
        let sidelength = dist(v[0].x, v[0].y, v[1].x, v[1].y);
        let r = w/sidelength;
        let xa = (v[1].x + v[2].x)/2;
        let ya = (v[1].y + v[2].y)/2;
        let D = ext + sidelength/2;
         
          // make vertex 0:
        let x0 = 2 * (D * v[0].x - ext * xa)/sidelength;
        let y0 = 2 * (D * v[0].y - ext * ya)/sidelength;
          
          // make vertex 1:
        let x1 = ((2*D - w)*v[0].x - 2*ext*xa + w*v[1].x)/sidelength;
        let y1 = ((2*D - w)*v[0].y - 2*ext*ya + w*v[1].y)/sidelength;
          
          // make vertex 2:
        let x2 = (1-r) * v[0].x + r * v[1].x;
        let y2 = (1-r) * v[0].y + r * v[1].y;
          
          // make vertex 3:
        let x3 = r * (v[1].x + v[2].x) + (1 - (2*r)) * v[0].x;
        let y3 = r * (v[1].y + v[2].y) + (1 - (2*r)) * v[0].y;
          //returnVal.addVertex(x3,y3);
          
          // make vertex 4:
        let x4 = (1-r) * v[0].x + r * v[2].x;
        let y4 = (1-r) * v[0].y + r * v[2].y;
          
          // make vertex 5:
        let x5 = ((2*D - w)*v[0].x - 2*ext*xa + w*v[2].x)/sidelength;
        let y5 = ((2*D - w)*v[0].y - 2*ext*ya + w*v[2].y)/sidelength;
          
        let h = []
        h.push({x:x4,y:y4});
        h.push({x:x5,y:y5});
        h.push({x:x0,y:y0});
        h.push({x:x1,y:y1});
        h.push({x:x2,y:y2});
        h.push({x:x3,y:y3});
        
        return new sixgon(h,false,10-d);
      
      }
      
    makePlate(v, ext, w, d){
      
        let sidelength = dist(v[0].x, v[0].y, v[1].x, v[1].y);
        let r = w/sidelength;
        let xa = (v[1].x + v[2].x)/2;
        let ya = (v[1].y + v[2].y)/2;
        let D = ext + sidelength/2;
      
        // make vertex 0:
        let x0 = 2 * (D * v[0].x - ext * xa)/sidelength;
        let y0 = 2 * (D * v[0].y - ext * ya)/sidelength;
        
        // make vertex 1:
        let xtemp = (1-r) * v[0].x + r * v[1].x;
        let ytemp = (1-r) * v[0].y + r * v[1].y;
        let x1 = x0 - v[0].x + xtemp;
        let y1 = y0 - v[0].y + ytemp;
        
        // make vertex 3:
        let x3 = 2*w*v[0].x/ext - (2*w-ext)*x0/ext;
        let y3 = 2*w*v[0].y/ext - (2*w-ext)*y0/ext;
        
        // make vertex 5:
        xtemp = (1-r) * v[0].x + r * v[2].x;
        ytemp = (1-r) * v[0].y + r * v[2].y;
        let x5 = x0 - v[0].x + xtemp;
        let y5 = y0 - v[0].y + ytemp;
        
        // make vertex 2:
        let x2 = 2*x1 - 2*x0+ x5;
        let y2 = 2*y1 - 2*y0+ y5;
      
        // make vertex 4:
        let x4 = 2*x5 - 2*x0 + x1;
        let y4 = 2*y5 - 2*y0 + y1;
      
        let h = []
        h.push({x:x4,y:y4});
        h.push({x:x5,y:y5});
        h.push({x:x0,y:y0});
        h.push({x:x1,y:y1});
        h.push({x:x2,y:y2});
        h.push({x:x3,y:y3});
        
        return new sixgon(h,true,10-d);
      
      }
      
    makeHex(p, d){
        let sidelength = dist(p[0].x, p[0].y, p[1].x, p[1].y);
        let w;
        let extent = (sidelength/2) + (3*random()*sidelength/2); 
        if (sidelength > 2*this.scale) {
             w = 2*random() * sidelength/3;
        }
        else {
             w = 3*random()*sidelength/2; 
        }
        if (w > sidelength) {
          return this.makePlate(p,extent,w,d);
        }
        else {
          return this.makeStick(p, extent, w, d);
        }
      }
      
    makeFlake(curr, depth){ // curr is a hexagon
        let retHexes = [];
        retHexes.push(curr);
        if (depth > 1){
          let verts = curr.vertices;
          let pilot1 = [];
          pilot1.push(verts[2]);
          pilot1.push(verts[3]);
          pilot1.push(verts[1]);
          let newFlake = this.makeHex(pilot1,depth-1);
          retHexes = retHexes.concat(this.makeFlake(newFlake,depth-1));
          if (curr.plate) {
            let pilot2 = [];
            pilot2.push(verts[1]);
            pilot2.push(verts[2]);
            pilot2.push(verts[0]);
            let newBranchSeed = this.makeHex(pilot2, depth-1);
            let newBranch = this.makeFlake(newBranchSeed, depth-1);
            retHexes = retHexes.concat(newBranch);
            let reflectBranch = this.buildReflection(newBranch,pilot1);
            retHexes = retHexes.concat(reflectBranch);
          }
        }
        //retHexes.push(curr);
        return retHexes;
      }
}

class sixgon {
  constructor(verts, plate, level) {
    this.col = color(50,0, 250, random(127)); 
    this.vertices = verts;
    this.plate = plate;
    this.level = level;
  }

  setColor(c){
    this.col = c;
  }

  display(){
    noStroke();
    fill(this.col);
    beginShape();
    for (let i = 0; i < this.vertices.length; i++) {
      vertex(this.vertices[i].x, this.vertices[i].y);
    }
    endShape(CLOSE);
  }
}
