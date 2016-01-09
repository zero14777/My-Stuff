import processing.core.*; 
import processing.xml.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class HW9_program_3 extends PApplet {

public void setup() {
  background(255);//set background to white
  size(400, 400);//set the size of the program window
  noStroke();//remove stroke
}
int x;//declare x
public void draw() {
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

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#F0F0F0", "HW9_program_3" });
  }
}
