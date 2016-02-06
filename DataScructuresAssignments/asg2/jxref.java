// $Id: jxref.java,v 1.22 2013-10-24 14:12:46-07 - - $

import java.io.*;
import java.util.Iterator;
import java.util.Map.Entry;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import static java.lang.System.*;

class jxref {
   private static final String STDIN_FILENAME = "-";
   private static final String REGEX = "\\w+([-'.:/]\\w+)*";
   private static final Pattern PATTERN = Pattern.compile(REGEX);

   private static void xref_file (String filename, Scanner file){
      listmap map = new listmap();
      //Prints out the file name for each file before they are sorted.
      System.out.println("::::::::::::::::::::::::::::::::");
      System.out.println(filename);
      System.out.println("::::::::::::::::::::::::::::::::");
      //Goes through each line of the file, takes words and puts them
      //in map. map.insert sorts the words
      for (int linenr = 1; file.hasNextLine(); ++linenr) {
         String line = file.nextLine();
         Matcher match = PATTERN.matcher (line);
         while (match.find()) {
            String word = match.group();
            map.insert(word, linenr);
         }
      }
      //Prints out words in ascending lexicographic order from top
      //to bottom. It will also print the number of times the word
      //appears, and the lines on which it appears.
      for (Entry<String, intqueue> entry: map) {
         Iterator<Integer> i = entry.getValue().iterator();
         System.out.print (entry.getKey() + " [" + 
              entry.getValue().getcount() + "]");
         for(int count = entry.getValue().getcount(); count > 0;
                                                        count--){
              System.out.print (" " + i.next());
         }
         System.out.println();
      }
   }

   // For each filename, open the file, cross reference it,
   // and print.
   private static void xref_filename (String filename) {
      if (filename.equals (STDIN_FILENAME)) {
         xref_file (filename, new Scanner (System.in));
      }else {
         try {
            Scanner file = new Scanner (new File (filename));
            xref_file (filename, file);
            file.close();
         }catch (IOException error) {
            misc.warn (error.getMessage());
         }
      }
   }

   // Main function scans arguments to cross reference files.
   public static void main (String[] args) {
      if (args.length == 0) {
         xref_filename (STDIN_FILENAME);
      }else {
         for (String filename: args) {
            xref_filename (filename);
         }
      }
      exit (misc.exit_status);
   }

}

