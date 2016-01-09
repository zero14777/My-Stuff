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

public class HW9_program_4 extends PApplet {

public void setup() {
  size(150, 150);//set the size of the program window
}
int x = 0;
public void draw() {
  x = x + 1;//adds 1 to the x counter each frame
  noStroke();
  background(0);//set background to black to show space
  fill(255);//set color of sprite to be white because that's the color they were in space invaders
  if (x <= 30) {//will show this image for 30 frames
    rect(20, 80, 10, 30);//10 pixel wide lines that make up the space invader sprite when its mouth is closed
    rect(30, 70, 10, 20);
    rect(40, 40, 10, 10);
    rect(40, 60, 10, 50);
    rect(50, 50, 10, 20);
    rect(50, 80, 10, 20);
    rect(50, 110, 10, 10);
    rect(60, 60, 10, 40);
    rect(60, 110, 10, 10);
    rect(70, 60, 10, 40);
    rect(80, 60, 10, 40);
    rect(80, 110, 10, 10);
    rect(90, 50, 10, 20);
    rect(90, 80, 10, 20);
    rect(90, 110, 10, 10);
    rect(100, 40, 10, 10);
    rect(100, 60, 10, 50);
    rect(110, 70, 10, 20);
    rect(120, 80, 10, 30);
  } 
  else {//show this image for thirty frames
    rect(20, 50, 10, 40);//10 pixel wide lines that make up the space invader sprite when its mouth is open
    rect(30, 70, 10, 30);
    rect(30, 110, 10, 10);
    rect(40, 40, 10, 10);
    rect(40, 60, 10, 50);
    rect(50, 50, 10, 20);
    rect(50, 80, 10, 20);
    rect(60, 60, 10, 40);
    rect(70, 60, 10, 40);
    rect(80, 60, 10, 40);
    rect(90, 50, 10, 20);
    rect(90, 80, 10, 20);
    rect(100, 40, 10, 10);
    rect(100, 60, 10, 50);
    rect(110, 70, 10, 30);
    rect(110, 110, 10, 10);
    rect(120, 50, 10, 40);
  }
  if (x == 60) {//resets the counter to 0 so that the animation cycles
    x = 0;
  }
}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#F0F0F0", "HW9_program_4" });
  }
}
