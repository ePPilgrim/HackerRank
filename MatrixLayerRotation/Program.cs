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
    private List<List<int>> matrix;
    private readonly int R;
    private readonly int RCnt;
    private readonly int CCnt;

    public Matrix(List<List<int>> matrix, int r){
        this.matrix = matrix;
        R = r;
        RCnt = matrix.Count();
        CCnt = matrix[0].Count();
    }

    public void SetOutlined(int rix, int cix, int level){
        var list = new List<int>(2048);
        foreach(var el in getOutlined(level)){
            list.Add(el);
        }

        int i = 0;
        var shiftedix = getShiftedIx(level); 
        foreach(var ix in generateIndexes(shiftedix.Item1, shiftedix.Item2, level)){
            matrix[ix.Item1][ix.Item2] = list[++i];
        }
    }

    public void Print(){
        foreach(var row in matrix){
            foreach(var el in row){
                Console.Write($"{el} ");
            }
            Console.Write("\n");
        }
    }

    private Tuple<int,int> getShiftedIx(int level)
    {
        int rr = RCnt - 2 * level;
        int cc = CCnt - 2 * level;
        int r = R % (rr + cc - 4); 
        if(r < rr){
            return new Tuple<int,int>(level + r, level);
        } else if(r < cc + rr){
            return new Tuple<int,int>(RCnt - 1 - level, level + r - rr);
        } else if(r < 2 * rr + cc){
            return new Tuple<int,int>(level + 2 * rr + cc - r, CCnt - level - 1);
        }
        return new Tuple<int,int>(level,level + 2 * (rr + cc) - r);
    }

    private IEnumerable<int> getOutlined(int level){
        var list = new List<int>();
        foreach(var el in generateIndexes(level, level,level)) {
            yield return matrix[el.Item1][el.Item2];
        }
    }

    private IEnumerable<Tuple<int,int>> generateIndexes(int rix, int cix, int level){
        var rc = (cix == level || cix == CCnt - level - 1) ? 0 : 1;
        var ix = new int[]{rix, cix};

        do{
            yield return new Tuple<int,int>(ix[0], ix[1]);
            if(ix[rc] == level || (ix[rc] + level + 1) == ((rc == 1) ? CCnt : RCnt) ){
                rc = (rc + 1) % 2;
            }
            ix[rc] -= 1;
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
