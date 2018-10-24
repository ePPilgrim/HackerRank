using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace PointsOnPlane
{
    class Program
    {
        public Program()
        {
            int n = 1 << 16;
            masks = new int[n][][];
            for(int i = 1; i < n; ++ i)
            {
                List<int[]> values = new List<int[]> { new int[2] { 0, 0 } };
                int init_pos = -1;
                for(var j = 0; j < 16; ++ j) {
                    if(((1 << j) & i) != 0) {
                        if (init_pos < 0) init_pos = j;
                        values.Add(new int[2] { init_pos, j });
                    }                   
                }
                masks[i] = values.ToArray();
            }
        }
        private int[,] sets;

        readonly int[][][] masks = null;



        private Comparison<int[]> comp = (x1, x2) => x1[0] < x2[0] ? -1 : (x1[0] == x2[0]) && (x1[1] < x2[1]) ? -1 : 1;
        private Func<int[], int[], bool> iscolinear = (p1, p2) => (p1[0] * p2[1] - p1[1] * p2[0]) == 0;
        private int encodeset(int i, int setid) => setid | (1 << i);

        public void Preprocessing(int[][] points)
        {
            Array.Sort(points, Comparer<int[]>.Create(comp));
            int n = points.Length;
            List<int> setids = new List<int> { 0 };
            for (int i = 0; i < n; ++i)
            {
                setids.Add(encodeset(i, 0));
                int baseid = sets[i, i] = setids.Count;
                for (int j = i + 1; j < n; ++j)
                {
                    int[] p1 = new int[2] { points[j][0] - points[i][0], points[j][1] - points[i][1] };
                    for (int k = 0; k < j; ++k)
                    {
                        if (k == i) continue;
                        int[] p2 = new int[2] { points[k][0] - points[i][0], points[k][1] - points[i][1] };
                        int id = sets[i, k];
                        if (!iscolinear(p1, p2)) {
                            setids.Add(setids[baseid]);
                            id = setids.Count;
                        }
                        setids[id] = encodeset(j, setids[id]);
                        sets[i, j] = sets[j, i] = id;
                    }
                }
            }
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    sets[i, j] = setids[sets[i, j]];
        }

        public int[] FindSolution()
        {
            int prime = 888888;
            int n = sets.GetLength(0);
            int power = 1 << n + 1;
            int[,] dp = new int[1 << n, 2];
            for (int i = 1; i < power; ++i)
            {
                foreach (var p in masks[i])
                {
                    int mask = sets[p[0], p[1]];
                    int subset = (i ^ mask) & i;
                    int newpower = 1 + dp[subset, 0];
                    if (newpower < dp[i, 0])
                    {
                        dp[i, 0] = newpower;
                        dp[i, 1] = (newpower * dp[subset, 1]) % prime;
                    } else if (newpower == dp[i, 0])
                        dp[i, 1] = (dp[i, 1] + dp[subset, 1] * newpower) % prime;

                }

            }
            return new int[2]{dp[n, 0],dp[n, 1]};
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Number = {0}",1<<3);
            int hhh = 9;
        }
    }
}
