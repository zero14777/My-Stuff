// $Id: bitree.java,v 1.3 2013-10-17 18:34:13-07 - - $
//  James Brower - jrbrower
//  Peter Greer-Berezovsky - pbgreerb
//
// NAME
//    class bitree - starter class for bitree implementation.
//

class bitree {
   char symbol;
   bitree left;
   bitree right;

   bitree (char symbol_, bitree left_, bitree right_) {
      symbol = symbol_;
      left = left_;
      right = right_;
   }

   bitree (char symbol_) {
      this (symbol_, null, null);
   }
   
   public String toString () {
      
      return "" + symbol;
   }
}

