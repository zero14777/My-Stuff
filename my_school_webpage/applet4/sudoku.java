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

public class sudoku extends PApplet {

public void setup() {
  size(270, 270);//sets the size of the window
  strokeWeight(1);//sets the weight of the lines that will border the cells
  cellarray();//call function to create the sudoku board
}

public void draw ( ) {
  int cell_y = (mouseY/30);//checks the y value of the cell the mouse is over
  int cell_x = (mouseX/30);//checks the x valie of the cell the mouse is over
  if (mousePressed) {
    fill(255, 0, 0);//sets fill to red
    ellipse(30*cell_x+30/2, 30*cell_y+30/2, 30/4, 30/4);//draws a red circle in the center of the cell the cursor is over
  }
}

int u = 0;//initializes and defines u

public void cellarray() {//creates a 3x3 array of 3x3 sets of cells
  for (int c = 0; c < 3; c++) {//controls which column the set of cells are created in
    for (int j = 0; j < 3; j++) {//controls which row the set of cell are created in
      if (u==0) {
        fill(255);//sets fill to white
        u++;//makes the fill switch to gray after a set of cells are white
      }
      else {
        fill(200);//sets fill to grey
        u=0;//makes the fill switch back to white after a set of cells are grey
      }
      triple(c*30, j*30);//creates a set of 3x3 cells
    }
  }

  strokeWeight(5);//sets stroke size to 5 pixels wide
  line(90, 0, 90, 270);//draws the left bold vertical line
  line(180, 0, 180, 270);//draws the right bold vertical line
  line(0, 90, 270, 90);//draws the upper bold horizontal line
  line(0, 180, 270, 180);//draws the lower bold horizontal line
  strokeWeight(1);//resets stroke size to 1 pixel wide
}

public void triple(int o, int k) {//makes a 3x3 array of 30x30 pixel sized cells
  for (int i = 0; i < 3; i++) {//controls which column the cell is created in
    for (int z = 0; z < 3; z++) {//controls which row the cell is created in
      int h = ((i*30)+(3*o));//calculates the x coordinate of the cell
      int b = ((z*30)+(3*k));//calculates the y coordinate of the cell
      cell(h, b);//calls function to create cells
    }
  }
}

public void cell(int m, int n) {
  rect(m, n, 30, 30);//creates an individual cell
}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#F0F0F0", "sudoku" });
  }
}
