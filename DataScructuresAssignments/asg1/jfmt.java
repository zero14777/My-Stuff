// Peter Greer-Berezovsky Pbgreerb
// James Robert Anderson Brower Jrbrower

// $Id: jfmt.java,v 1.4 2013-10-13 01:52:04-07 - - $
//
// NAME
//    jfmt - simple text formatter
//
// SYNOPSIS
//    jfmt [-width] [filename...]
//
// DESCRIPTION
//    This program reads in text lines from input files and writes its
//    output to stdout. Error messages are written to stderr consecutive
//    sequences of lines containing non-whitespace characters are
//    considered as a single paragraph. A paragraph is written out with
//    a maximal sequence of words not to exceed the specified output
//    line width. It is then followed by one empty line
//

import java.io.*;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import static java.lang.System.*;

class jfmt {
   // Static variables keeping the general status of the program.
   public static final String JAR_NAME = get_jarname();
   public static final int EXIT_SUCCESS = 0;
   public static final int EXIT_FAILURE = 1;
   public static int exit_status = EXIT_SUCCESS;

   // A basename is the final component of a pathname.
   // If a java program is run from a jar, the classpath is the
   // pathname of the jar.
   static String get_jarname() {
      String jarpath = getProperty ("java.class.path");
      int lastslash = jarpath.lastIndexOf ('/');
      if (lastslash < 0) return jarpath;
      return jarpath.substring (lastslash + 1);
   }

   // Formats a single file.
   static void format (Scanner infile, int width) {
      
      // Create a LinkedList of Strings.

      List<String> words = new LinkedList<String>();

      // Read each line from the opened file, one after the other.
      // Stop the loop at end of file.
      for (int linenr = 1; infile.hasNextLine(); ++linenr) {
         String line = infile.nextLine();
         int wpl = 0;

         // Split the line into words around white space and iterate
         // over the words.
         for (String word: line.split ("\\s+")) {

            // Skip an empty word if such is found.
            if (word.length() == 0) continue;
            // Append the word to the end of the linked list.
            words.add (word);
            wpl++;
            }
         // prints and clears any the stored text if there is a blank
         // line in a file being read
         if (wpl == 0 && !(words.isEmpty())){
            print_paragraph (words, width);
            
         }
      }
      // Prints and clears any stored text left at the end of reading a
      // file
      if (!(words.isEmpty())){
         print_paragraph (words, width);
      }

   }
   // Prints out all of the words currently stored in the words list in
   // order, checking to make sure that no line exceeds the designated
   // width.
   public static void print_paragraph
      (List<String> words, int lineWidth){
         
         for(int listElement = 0; listElement < words.size();){
            // Adds a blank line between paragraphs
            out.printf ("%n");
            int width = lineWidth;
            out.printf ("%s", words.get(listElement));
            width = width - words.get(listElement).length();
            if (listElement < (words.size() - 1)){
               listElement++;
               } else {
                  break;
                  }
            while((width - words.get(listElement).length() - 1) >= 0){
               width = width - words.get(listElement).length() - 1;
               out.printf (" %s", words.get(listElement));
               if (listElement < (words.size() - 1)){
                  listElement++;
                  } else {
                     break;
                     }
            }
         }
         // Clears out the list
         words.clear();
      }

   // Main function scans arguments and opens/closes files.
   public static void main (String[] args) {
      int width = 65;
      if (args.length == 0) {
         // There are no filenames given on the command line.
         out.printf ("FILE: -%n");
         format (new Scanner (in), width);
      }else {
         // Iterate over each filename given on the command line.
         for (int argix = 0; argix < args.length; ++argix) {
            if (args[0].charAt(0) == '-') {
               // Looks for a custom width given when the program is
               // run.
               try {
                  width =
                   Integer.parseInt(args[0].replaceAll("[\\D]", ""));
               // Provides an error message if someone formats
               // the option
               // incorrectly when trying to run the program
               }catch (NumberFormatException error) {
                  exit_status = EXIT_FAILURE;
                  err.printf("Usage: jfmt [-width] [filename...]%n");
                  break;
                  }
               argix++;
            }
            String filename = args[argix];
            if (filename.equals ("-")) {
               // Treat a filename of "-" to mean System.in.
               out.printf ("FILE: -%n");
               format (new Scanner (in), width);
            }else {
               // Open the file and read it, or error out.
               try {
                  Scanner infile = new Scanner (new File (filename));
                  format (infile, width);
                  infile.close();
               }catch (IOException error) {
                  exit_status = EXIT_FAILURE;
                  err.printf ("%s: %s%n", JAR_NAME,
                              error.getMessage());
               }
            }
         }
      }
      exit (exit_status);
   }

}
