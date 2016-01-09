// $Id: exit1.java,v 1.3 2013-10-10 15:03:51-07 - - $
//
// NAME
//     exit1
//
// SYNOPSIS
//     EXIT 1
//
// DESCRIPTION
//     Prints "EXIT 1" to the standard error amd then exits with a status of 1

import java.io.*;

class exit1 {

   public static void main (String[] args) {
      System.err.println("EXIT 1");
      System.exit(1);
   }
}
