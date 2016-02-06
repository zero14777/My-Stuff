void setup() {
  background(255);//set background to white
  size(400, 400);//set the size of the program window
  noStroke();//remove stroke
}
int x;//declare x
void draw() {
  if (x == 0) {
    for (int i = 1; i <= 10; i++) {//creates 10 triangles of random size, color and location
      fill(random(0, 255), random(0, 255), random(0, 255), 155);//selects a random color that will fill the next triangle made
      triangle(random(0, 400), random(0, 400), random(0, 400), random(0, 400), random(0, 400), random(0, 400));//makes 10 triangles with random sizes and locations
    }
    x++;
  }
  if (mousePressed) {//clears the 10 existing triangles and makes 10 new ones
    background(255);//resets background to white
    x = 0;//resets x to zero
  }
}

