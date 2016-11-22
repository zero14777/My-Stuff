using System;
using System.Collections.Generic;
using System.IO;
class Solution {
    static void Main(String[] args) {
        char[] delimiterChar = {'\t'};
        string line;
        float prev_temp_diff = 0;
        
        Console.ReadLine();
        Console.ReadLine();
        
        while ((line = Console.ReadLine()) != null) {
            string[] line_data = line.Split(delimiterChar);
            
            if (line_data[2][0] == 'M') {
                Console.WriteLine(float.Parse(line_data[3]) + prev_temp_diff);
            } else if (line_data[3][0] == 'M') {
                Console.WriteLine(float.Parse(line_data[2]) - prev_temp_diff);
            } else {
                prev_temp_diff = float.Parse(line_data[2]) - float.Parse(line_data[3]);
            }
        }
    }
}