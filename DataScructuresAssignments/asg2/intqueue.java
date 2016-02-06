// $Id: intqueue.java,v 1.3 2013-10-24 14:28:58-07 - - $

import java.util.Iterator;
import java.util.NoSuchElementException;

class intqueue implements Iterable<Integer> {

   private class node {
      int linenr;
      node link;
   }
   private int count = 0;
   private node front = null;
   private node rear = null;
   
   //Adds a line number on which the intqueue's corresponding word
   //appears and increments the count to display the number of times
   //the intqueue's corresponding word appears.
   public void insert (int number) {
      ++count;
      //Determines then places the line number in its proper position
      //in the linked list.
      if(front == null){
          front = new node();
          front.linenr = number;
      } else if(rear == null){
          rear = new node();
          rear.linenr = number;
          front.link = rear;
      } else {
          node newNode = new node();
          newNode.linenr = number;
          rear.link = newNode;
          rear = newNode;
      }
   }

   public boolean empty() {
      return count == 0;
   }

   public int getcount() {
      return count;
   }

   public Iterator<Integer> iterator() {
      return new iterator();
   }

   private class iterator implements Iterator<Integer> {
      node curr = front;

      public boolean hasNext() {
         return curr != null;
      }

      public Integer next() {
         if (curr == null) throw new NoSuchElementException();
         Integer next = curr.linenr;
         curr = curr.link;
         return next;
      }

      public void remove() {
         throw new UnsupportedOperationException();
      }
   }

}

