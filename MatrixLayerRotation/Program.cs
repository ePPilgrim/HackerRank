﻿using System.CodeDom.Compiler;
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
using System.Xml.Schema;

namespace MatrixReduction {
    internal class Matrix {
        private List<List<int>> matrix;
        private readonly int R;
        private readonly int RCnt;
        private readonly int CCnt;
        public int MaxLevel;

        public Matrix(List<List<int>> matrix, int r) {
            this.matrix = matrix;
            R = r;
            RCnt = matrix.Count();
            CCnt = matrix[0].Count();
            MaxLevel = Math.Min(RCnt, CCnt)/2;
        }

        public void SetOutlined(int level) {
            var list = new List<int>(2048);
            foreach (var el in getOutlined(level)) {
                list.Add(el);
            }

            int i = 0;
            var shiftedix = getShiftedIx(level);
            int rix = shiftedix.Item1;
            int cix = shiftedix.Item2;
            foreach (var ixs in generateIndexes(rix, cix, level)) {
                matrix[ixs.Item1][ixs.Item2] = list[i++];
            }
        }

        public void Print() {
            foreach (var row in matrix) {
                foreach (var el in row) {
                    Console.Write($"{el} ");
                }
                Console.Write("\n");
            }
        }

        private Tuple<int, int> getShiftedIx(int level) {
            int rr = RCnt - 2 * level;
            int cc = CCnt - 2 * level;
            int r = R % (2 * (rr + cc) - 4);
            if (r < rr) {
                return new Tuple<int, int>(level + r, level);
            }
            if (r < cc + rr) {
                return new Tuple<int, int>(RCnt - 1 - level, level + r - rr);
            } 
            if (r < 2 * rr + cc) {
                return new Tuple<int, int>(level + 2 * rr + cc - r, CCnt - level - 1);
            }
            return new Tuple<int, int>(level, level + 2 * (rr + cc) - r);
        }

        private IEnumerable<int> getOutlined(int level) {
            int cix = level;
            int rix = level;
            foreach (var ixs in generateIndexes(rix, cix, level)) {
                yield return matrix[ixs.Item1][ixs.Item2];
            }
        }

        private IEnumerable<Tuple<int,int>> generateIndexes(int rix, int cix, int level) {
            int min = level;
            int rmax = RCnt - min - 1;
            int cmax = CCnt - min - 1;
            int rx = rix;
            int cx = cix;

            do {
                yield return new Tuple<int,int>(rx,cx);
                if (rx == min && cx < cmax)  ++cx; 
                else if (cx == cmax && rx < rmax) ++rx;
                else if (rx == rmax && cx > min) --cx;
                else --rx;
            }
            while ((rx != rix) || (cx != cix));
        }

        public void getOutlined1() {
            int cix = 0;
            int rix = 1;
            foreach (var ix in generateIndexes(rix, cix, 0)) {
                Console.WriteLine($"{ix.Item1} {ix.Item2}");
            }
        }


    }

    class Solution {

        // Complete the matrixRotation function below.
        static void matrixRotation(List<List<int>> matrix, int r) {
            var mat = new Matrix(matrix, r);
            mat.Print();
            for (int i = 0; i < mat.MaxLevel; ++i) {
                mat.SetOutlined(i);
            }
            mat.Print();

        }

        static void Main(string[] args) {
            //string[] mnr = Console.ReadLine().TrimEnd().Split(' ');

            //int m = Convert.ToInt32(mnr[0]);

            //int n = Convert.ToInt32(mnr[1]);

            //int r = Convert.ToInt32(mnr[2]);

            //List<List<int>> matrix = new List<List<int>>();

            //for (int i = 0; i < m; i++) {
            //    matrix.Add(Console.ReadLine().TrimEnd().Split(' ').ToList().Select(matrixTemp => Convert.ToInt32(matrixTemp)).ToList());
            //}
            int r = 9;
            var matrix = new List<List<int>>() {new List<int>() {1,2,3},new List<int>() {4,5,6} };
            matrixRotation(matrix, r);
        }
    }

}



