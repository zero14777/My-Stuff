import java.io.*;
import java.util.Scanner;
import java.util.NoSuchElementException;
import java.lang.*;

class bitreecalc {
   private static final String STDIN_FILENAME = "-";

   public static void main (String[] args) {
      if (args.length == 0) {
         calc_filename (STDIN_FILENAME);
      }else {
         for (String filename: args) {
            calc_filename (filename);
         }
      }
      System.exit (auxlib.exitvalue);

   }

   private static void calc_filename (String filename) {
      if (filename.equals (STDIN_FILENAME)) {
         parse (new Scanner (System.in));
      }else {
         try {
            Scanner file = new Scanner (new File (filename));
            parse (file);
            file.close();
         }catch (IOException error) {
            auxlib.warn (error.getMessage());
         }
      }
   }


   private static char firstChar (String line) {
      for (int i = 0; i < line.length(); i++) {
         switch (line.charAt(i)) {
            case ' ':  continue;
            case '\t': continue;
            default:   return line.charAt(i);
         }
      }
      return ' ';
   }

   private static char findOperator (String line) {
      for (int i = 0; i < line.length(); i++) {
         if (line.charAt(i) == '='
             || line.charAt(i) == ':'
             || line.charAt(i) == '?')
            return line.charAt(i);
      }
      return ' ';
   }

   private static void parse(Scanner input) {
      int lineNumber = 0;
      while (input.hasNextLine()) {
         lineNumber++;
         String line = input.nextLine();
         System.out.println(line);
         char variable = firstChar(line);
         if (!Character.isLetter(variable)) {
            //do dat gey shit with the auxlib.yava across the street form the mothafucka
            continue;
         }
         char operator = findOperator(line);
         switch (operator) {
            case '=': String tString = line.substring((indexOf("=")+1));
                      makeTree(variable, tString);
                      break;
            case ':': String dString = line.substring((indexOf(":")+1));
                      double varVal = Double.parseDouble(dstring);
                      giveValue(variable, varVal);
                      break;
            case '?': printValue(variable);
                      break;
            default:  System.err.println("No operator");
                      break;
         }
      }
   }

   private static void printValue(char variable){
      System.out.println(variable + ": " + symbol_table.get_value(variable));
      System.out.println("  " + symbol_table.get_tree(variable).toString());
   }
   
   private static void giveValue(char variable, double varVal){
      symbol_table.put(variable, varVal, null);
      printValue(variable);
   }

   private void makeTree(char variable, String tString){
      for(int i = 0; i < tString.length; i++){
         if(tString.chatAt(i).isLetter() || 
           tString.chatAt(i) == '+' ||
           tString.chatAt(i) == '-' ||
           tString.chatAt(i) == '*' ||
           tString.chatAt(i) == '/')
            linked_stack.push(tString.charAt(i));
      }
      symbol_table.get_tree(variable).symbol = linked_stack.pop();
      symbol_table.get_tree(variable).right = branch();
      symbol_table.get_tree(variable).left = branch();
   }

   private bitree branch(){
      char symbol = linked_stack.pop();
      bitree left = null;
      bitree right = null;
         if(!Character.isLetter(symbol)){
            right = branch();
            left = branch();
      }
      bitree branch = new bitree(symbol, left, right);
      return branch;
   }

   private double getTreeVal(bitree branch){
      if(Character.isLetter(branch.symbol)){
         return symbol_table.get_value(branch.symbol);
      }
      double leftVal = getTreeVal(branch.left);
      double rightVal = getTreeVal(branch.right);
      switch(branch.symbol){
         case '+': return (leftVal + rightVal);
                   break;
         case '-': return (leftVal - rightVal);
                   break;
         case '*': return (leftVal * rightVal);
                   break;
         case '/': return (leftVal / rightVal);
                   break;
         default:  break;
      }
   }
}
