using System;
using System.Collections.Generic;
using System.IO;
class Solution {
    static void Main(String[] args) {
        /* Enter your code here. Read input from STDIN. Print output to STDOUT. Your class should be named Solution */
        string input = Console.ReadLine();
        int[] characters = new int[26];
        
        for (int i = 0; i < 26; i++) {
            characters[i] = 0;
        }
        
        for (int i = 0; i < input.Length; i++) {
            characters[input[i] - 97]++;
        }
        
        int too_many_odds = input.Length % 2;
        
        for (int i = 0; i < 26; i++) {
            if (characters[i] % 2 == 1) {
                too_many_odds--;
            }
        }
        
        if (too_many_odds < 0) {
            Console.WriteLine("NO");
        } else {
            Console.WriteLine("YES");
        }
    }
}