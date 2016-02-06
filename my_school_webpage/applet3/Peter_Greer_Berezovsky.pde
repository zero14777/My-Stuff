
int x = 0, y = 0, z = 0, a = 0;//declaring and initializing variables
void setup() {
  size(800, 200);
}

void draw() {
  noStroke();
  background(0);
  fill(255, 0, 0);//Blinky is red by default
  if(mousePressed){//If the mouse is held down Blinky turns yellow 
    fill(255, 255, 0);
  }
  rect(50 + x, 70, 10, 90);//from left bar 1
  rect(60 + x, 40, 10, 130);//from left bar 2
  rect(70 + x, 30, 10, 140);//from left bar 3
  rect(80 + x, 20, 10, 140);//from left bar 4
  rect(90 + x, 20, 10, 130);//from left bar 5
  rect(100 + x, 10, 10, 150);//from left bar 6
  rect(110 + x, 10, 10, 160);//from left bar 7
  rect(120 + x, 10, 10, 160);//from left bar 8
  rect(130 + x, 10, 10, 150);//from left bar 9
  rect(140 + x, 20, 10, 130);//from left bar 10
  rect(150 + x, 20, 10, 140);//from left bar 11
  rect(160 + x, 30, 10, 140);//from left bar 12
  rect(170 + x, 40, 10, 130);//from left bar 13
  rect(180 + x, 70, 10, 90);//from left bar 14
  fill(255);//white
  rect(60 + x, 50, 40, 30);//left eye white part 1
  rect(70 + x, 40, 20, 50);//left eye white part 2
  rect(120 + x, 50, 40, 30);//right eye white part 1
  rect(130 + x, 40, 20, 50);//right eye white part 2
  fill(0, 0, 255);//blue
  rect(60 + x + y, 60 + a, 20, 20);//left eye blue part
  rect(120 + x + z, 60 + a, 20, 20);//right eye blue part
  if (x < mouseX-120) {//Makes Blinky move right
    x = x+1;
  }
  else if (x > mouseX-120) {//Makes Blinky move left
    x = x-1;
  }
  if (mouseX < (80 + x + y) && (y) != (0)) {//Makes the left blue part of the eye follow the mouse left
    y = y - 1;
  }
  if (mouseX > (60 + x + y) && (y) != (20)) {//Makes the left blue part of the eye follow the mouse right
    y = y + 1;
  }
  if (mouseX < (120 + x + z) && (z) != (0)) {//Makes the right blue part of the eye follow the mouse left
    z = z - 1;
  }
  if (mouseX > (140 + x + z) && (z) != (20)) {//Makes the right blue part of the eye follow the mouse right
    z = z + 1;
  }
  if (mouseY < (50) && (60 + a) != (50)) {//Make blue parts of the eyes follow the mouse up
    a = a - 1;
  }
  if (mouseY > (50) && (50 + a) != (50)) {//Make blue parts of the eyes follow the mouse down
    a = a + 1;
  }
}

