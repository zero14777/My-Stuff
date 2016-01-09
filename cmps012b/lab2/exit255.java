// $Id: exit255.java,v 1.1 2013-10-10 15:31:13-07 - - $
//
// NAME
//     exit255
//
// SYNOPSIS
//     EXIT 255
//
// DESCRIPTION
//     Prints "EXIT 255" to the standard error amd then exits with a status of 1

import java.io.*;

class exit255 {

   public static void main (String[] args) {
      System.err.println("EXIT 255");
      System.exit(255);
   }
}
