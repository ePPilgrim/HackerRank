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
            return find_solution();
        }

        private Comparison<int[]> comp = (x1, x2) => x1[0] < x2[0] ? -1 : (x1[0] == x2[0]) && (x1[1] < x2[1]) ? -1 : 1;
        private Func<int[], int[], bool> iscolinear = (p1, p2) => (p1[0] * p2[1] - p1[1] * p2[0]) == 0;
        private int encodeset(int i, int setid) => setid | (1 << i);

        private const int prime = 1000000007;

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
                    if(id == 0){
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
                int j0 = -1;
                for (var j = 0; j < 16; ++j) {
                    if (((1 << j) & i) != 0) {
                        if (j0 < 0) j0 = j;
                        tmpvals.Add(new int[4] { j0, j,(~((1 << j0)-1))&((1 << j + 1) - 1), (1 << j0) | (1 << j) });
                    }
                }
                _masks[i] = tmpvals.ToArray();
            }
            return _masks;
        }

        private int[] find_solution() {
            int n = 1 << sets.GetLength(0);
            int[,] dp = new int[n, 2];
            for (int i = 1; i < n; ++i) { dp[i, 0] = prime; dp[i, 1] = 0; }
            dp[0, 1] = 1;
            for (int i = 1; i < n; ++i) {
                foreach (var p in masks[i]){
                    int mask = p[2] & sets[p[0], p[1]];
                    int subset = i ^ mask;
                    int m = 1 + dp[subset, 0];
                    if (m < dp[i, 0]) {
                        dp[i, 0] = m;
                        dp[i, 1] = (m * dp[subset, 1]) % prime;
                    }
                    else if (dp[i, 0] == m) {
                        dp[i, 1] = (dp[i, 1] + dp[subset, 1] * m) % prime;
                        int subset2 = p[3] ^ i;
                        if((subset!= subset2) && (dp[subset2,0] == m - 1))
                            dp[i, 1] = (dp[i, 1] + dp[subset2, 1] * m) % prime;
                    }
                }
            }
            return new int[2]{dp[n - 1, 0],dp[n - 1, 1]};
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
                //4 168
                //textWriter.WriteLine(string.Join(" ", result));
            }

            ////textWriter.Flush();
            // textWriter.Close();
        }
    }
}
