using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text.RegularExpressions;
using System.Text;
using System;

internal class Matrix{
    public List<List<int>> matrix;
    public int R;
    public Matrix(List<List<int>> matrix, int r){
        this.matrix = matrix;
        R = r;
    }

    private IEnumerable<Tuple<int,int>> generateIndexes(int rix, int cix, int minix, int maxix, int dir){
        var rc = (cix == minix || cix == maxix) ? 0 : 1;
        var ix = new int[]{rix, cix};

        do{
            yield return new Tuple<int,int>(ix[0], ix[1]);
            if(ix[rc] == minix || ix[rc] == maxix){
                rc = (rc + 1) % 2;
            }
            ix[rc] += dir;
        } while(ix[0] != rix && ix[1] != cix);
    }
}

class Solution {

    // Complete the matrixRotation function below.
    static void matrixRotation(List<List<int>> matrix, int r) {


    }

    static void Main(string[] args) {
        string[] mnr = Console.ReadLine().TrimEnd().Split(' ');

        int m = Convert.ToInt32(mnr[0]);

        int n = Convert.ToInt32(mnr[1]);

        int r = Convert.ToInt32(mnr[2]);

        List<List<int>> matrix = new List<List<int>>();

        for (int i = 0; i < m; i++) {
            matrix.Add(Console.ReadLine().TrimEnd().Split(' ').ToList().Select(matrixTemp => Convert.ToInt32(matrixTemp)).ToList());
        }

        matrixRotation(matrix, r);
    }
}
