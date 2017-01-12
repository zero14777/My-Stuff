/*
#!/bin/python3
#Given an -digit positive integer, count and print the number of multiples of  that can be formed by concatenating subsequences of the given number's digits, modulo .

#Input Format

#The first line contains an integer denoting . 
#The second line contains a string describing an -digit integer.
*/

import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;
import java.math.BigInteger;

public class Solution {

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        String number = in.next();
        
        //keep track of all possible subsequences with a third least sig digit that is odd or even
        BigInteger odds = new BigInteger("0");
        BigInteger even = new BigInteger("0");

        //keep track of how many subsequences can be made using the various even digits
        BigInteger acc08 = new BigInteger("0");
        BigInteger acc6 = new BigInteger("0");
        BigInteger acc4 = new BigInteger("0");
        BigInteger acc2 = new BigInteger("0");

        //keep track of digit position we are looking at
        BigInteger one = new BigInteger("1");
        BigInteger mod_num = new BigInteger("1000000007");
        BigInteger output = new BigInteger("0");
        
        for(int c = 0; c < number.length(); c++){
            if (Character.getNumericValue(number.charAt(c)) == 0 || Character.getNumericValue(number.charAt(c)) == 8) {
                output = output.add(one);
                output = output.add(acc08);
                acc08 = acc08.add(one);
                acc08 = acc08.add(even);
                acc4 = acc4.add(odds);
            } else if (Character.getNumericValue(number.charAt(c)) == 6) {
                output = output.add(acc6);
                acc08 = acc08.add(odds);
                acc4 = acc4.add(one);
                acc4 = acc4.add(even);
            } else if (Character.getNumericValue(number.charAt(c)) == 4) {
                output = output.add(acc4);
                acc08 = acc08.add(one);
                acc08 = acc08.add(even);
                acc4 = acc4.add(odds);
            } else if (Character.getNumericValue(number.charAt(c)) == 2) {
                output = output.add(acc2);
                acc08 = acc08.add(odds);
                acc4 = acc4.add(one);
                acc4 = acc4.add(even);
            } else if(Character.getNumericValue(number.charAt(c)) == 3 || Character.getNumericValue(number.charAt(c)) == 7) {
                acc2 = acc2.add(one);
                acc2 = acc2.add(even);
                acc6 = acc6.add(odds);
            } else { //1 or 5 or 9
                acc2 = acc2.add(odds);
                acc6 = acc6.add(one);
                acc6 = acc6.add(even);
            }
            if (Character.getNumericValue(number.charAt(c)) % 2 == 1) {
                BigInteger temp1 = new BigInteger("2");
                BigInteger temp3 = temp1.pow(c);
                odds = odds.add(temp3);
            } else {
                BigInteger temp1 = new BigInteger("2");
                BigInteger temp3 = temp1.pow(c);
                even = even.add(temp3);
            }
            output = output.mod(mod_num);
        }
        System.out.println(output);
    }
}