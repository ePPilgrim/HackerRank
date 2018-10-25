using System;
using System.Collections.Generic;
using System.IO;


namespace PointsOnPlane
{
    class PointsOnPlane
    {
        public PointsOnPlane() { masks = init_masks(); }

        public int[] GetSolution(int[][] points) {
            init_sets(points);
            int n = init_dp();
            if(dp[n,0] == 1)
            {
                return new int[2] { dp[n, 0], dp[n, 1] };
            }
            return find_solution();
        }

        private Comparison<int[]> comp = (x1, x2) => x1[0] < x2[0] ? -1 : (x1[0] == x2[0]) && (x1[1] < x2[1]) ? -1 : 1;
        private Func<int[], int[], bool> iscolinear = (p1, p2) => (p1[0] * p2[1] - p1[1] * p2[0]) == 0;
        private int encodeset(int i, int setid) => setid | (1 << i);

        private const int prime = 1000000007;

        private int[,] dp = null;
        private int init_dp()
        {
            int n = 1 << sets.GetLength(1);
            dp = new int[n, 2];
            for(int i = 0; i < n; ++ i) { dp[i, 0] = prime; dp[i, 1] = 0; }
            dp[0, 1] = 1;
            for (int i = 0; i < sets.GetLength(0); ++i)
                for (int j = i; j < sets.GetLength(1); ++j)
                    dp[sets[i, j], 0] = dp[sets[i, j], 1] = 1;
            return n - 1;
        }

        private int[,] sets = null;
        private void init_sets(int[][] points) {
            Array.Sort(points, Comparer<int[]>.Create(comp));
            int n = points.Length;
            sets = new int[n, n];
            List<int> setids = new List<int>();
            for (int i = 0; i < n; ++i) {
                setids.Add(encodeset(i, 0));
                int baseid = sets[i, i] = setids.Count;
                for (int j = i + 1; j < n; ++j) {
                    int[] p1 = new int[2] { points[j][0] - points[i][0], points[j][1] - points[i][1] };
                    int id = 0;
                    for (int k = 0; k < j; ++k) {
                        if (k == i) continue;
                        int[] p2 = new int[2] { points[k][0] - points[i][0], points[k][1] - points[i][1] };
                        if (iscolinear(p1, p2)) {
                            id = sets[i, k];
                            setids[id - 1] = encodeset(j, setids[id - 1]);
                            break;
                        }
                    }
                    if(id == 0)
                    {
                        setids.Add(encodeset(j, setids[baseid - 1]));
                        id = setids.Count;
                    }
                    sets[i, j] = sets[j, i] = id;
                }
            }
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    sets[i, j] = setids[sets[i, j] - 1];
        }

        readonly private int[][][] masks = null;
        private static int[][][] init_masks() {
            int n = 1 << 16;
            int[][][] _masks = new int[n][][];
            for (int i = 1; i < n; ++i) {
                List<int[]> tmpvals = new List<int[]>();
                int init_pos = -1;
                for (var j = 0; j < 16; ++j) {
                    if (((1 << j) & i) != 0) {
                        if (init_pos < 0) init_pos = j;
                        tmpvals.Add(new int[2] { init_pos, j });
                    }
                }
                _masks[i] = tmpvals.ToArray();
            }
            return _masks;
        }

        private int[] find_solution() {
            int power = 1 << sets.GetLength(0);
            int[,] dp = new int[power, 2];
            for (int i = 0; i < power; ++i) { dp[i, 0] = prime; dp[i, 1] = 0; }
            dp[0, 1] = 1;
            dp[0, 0] = 0;
            for (int i = 1; i < power; ++i)
            {
                //if (dp[i, 0] < prime) continue;
                if(i == 7)
                {
                    int sss = 99;
                }
                foreach (var p in masks[i])
                {
                    int mask = sets[p[0], p[1]];
                    int subset = (i ^ mask) & i;
                    int newpower = 1 + dp[subset, 0];
                    if (newpower < dp[i, 0])
                    {
                        dp[i, 0] = newpower;
                        dp[i, 1] = ((newpower) * dp[subset, 1]) % prime;
                        if (subset == 0) dp[i, 1] = 1;
                    }
                    else if (newpower == dp[i, 0])
                    {
                        int tmp = 1;
                        if (subset != 0) tmp = (dp[subset, 1] * (newpower)) % prime;
                        dp[i, 1] = (dp[i, 1] + tmp) % prime;
                    }
                }
            }
            return new int[2]{dp[power - 1, 0],dp[power - 1, 1]};
        }

        static void Main(string[] args)
        {
            //TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);
            StreamReader sr = new StreamReader("C:\\Users\\PLDD\\Repa\\HackerRank\\PointsOnPlane\\input.txt");
            //int t = Convert.ToInt32(Console.ReadLine());
            int t = Convert.ToInt32(sr.ReadLine());

            var res = new PointsOnPlane();

            for (int tItr = 0; tItr < t; tItr++) {
                //int n = Convert.ToInt32(Console.ReadLine());
                int n = Convert.ToInt32(sr.ReadLine());
                int[][] coordinates = new int[n][];

                for (int coordinatesRowItr = 0; coordinatesRowItr < n; coordinatesRowItr++) {
                    coordinates[coordinatesRowItr] = Array.ConvertAll(sr.ReadLine().Split(' '), coordinatesTemp => Convert.ToInt32(coordinatesTemp));
                }

                int[] result = res.GetSolution(coordinates);

                Console.WriteLine("{0} {1}", result[0], result[1]);

                //textWriter.WriteLine(string.Join(" ", result));
            }

            ////textWriter.Flush();
           // textWriter.Close();
        }
    }
}
