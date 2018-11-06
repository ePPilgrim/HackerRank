using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

class Solution {




    /*
     * Complete the pointsInPlane function below.
     */
    static int[] pointsInPlane(int[][] coordinates) {
        int n = coordinates.Length;
        int group = 1;
        //int[,] lines = new int[n, n];
        //for (int i = 0; i < n; ++i) {
        //    int xi = coordinates[i][0];
        //    int yi = coordinates[i][1];
        //    for (int j = i + 1; j < n; ++j) {
        //        int xj = coordinates[j][0] - xi;
        //        int yj = coordinates[j][1] - yi;
        //        for(int k = 0; k < j; ++k) {
        //           var fff =  (coordinates[k][0] - xi) * yj - (coordinates[k][1] - yi) * xj;  
        //        }



        //    }

            return new int[10];

    }

    static void Main(string[] args) {
        int[][] arr =  { new int[]{1,3 }, new int[]{1,1}, new int[]{1,2 },
                                   new int[]{2,6 }, new int[]{2,1 } };
        var comp = Comparer<int[]>.Create((x, y) => x[0] < y[0] ? -1 : (x[0] == y[0] && x[1] < y[1]) ? -1 : 1);
        Array.Sort(arr, comp);
        int sss = 0;

        //TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        //int t = Convert.ToInt32(Console.ReadLine());

        //for (int tItr = 0; tItr < t; tItr++) {
        //    int n = Convert.ToInt32(Console.ReadLine());

        //    int[][] coordinates = new int[n][];

        //    for (int coordinatesRowItr = 0; coordinatesRowItr < n; coordinatesRowItr++) {
        //        coordinates[coordinatesRowItr] = Array.ConvertAll(Console.ReadLine().Split(' '), coordinatesTemp => Convert.ToInt32(coordinatesTemp));
        //    }

        //    int[] result = pointsInPlane(coordinates);

        //    textWriter.WriteLine(string.Join(" ", result));
        //}

        //textWriter.Flush();
        //textWriter.Close();
    }
}
