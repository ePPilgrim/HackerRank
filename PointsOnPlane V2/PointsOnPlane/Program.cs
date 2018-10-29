using System;
using System.Collections.Generic;
using System.IO;


namespace PointsOnPlane
{
    class PointsOnPlane
    {
        public PointsOnPlane() { fac = init_fac(); masks = init_masks(); }

        public int[] GetSolution(int[][] points) {
            init_sets(points);
            return find_solution(points.Length);
        }

        private void init_sets(int[][] points){
            Array.Sort(points, Comparer<int[]>.Create(comp));
            int n = points.Length;
            int[,] subsets = new int[n,n];
            List<int> setids = new List<int>();
            for (int i = 0; i < n; ++i){
                setids.Add(encodeset(i, 0));
                int baseid = subsets[i,i] = setids.Count;
                for (int j = i + 1; j < n; ++ j) {
                    int[] p1 = new int[2] { points[j][0] - points[i][0], points[j][1] - points[i][1] };
                    bool isfound = false;
                    for (int k = (i != 0) ? 0 : 1; k < j; k += (k + 1 != i) ? 1 : 2) {
                        int[] p2 = new int[2] { points[k][0] - points[i][0], points[k][1] - points[i][1] };
                        isfound = (p1[0] * p2[1] - p1[1] * p2[0]) == 0;
                        if (isfound) {
                            subsets[i,j] =subsets[j,i]= subsets[i,k];
                            setids[subsets[i,j] - 1] = encodeset(j, setids[subsets[i,j] - 1]);
                            break;
                        }
                    }
                    if (isfound == false) {
                        setids.Add(encodeset(j, setids[baseid - 1]));
                        subsets[i,j] = subsets[j,i] = setids.Count;
                    }
                }
            }

            int[] lineseg = new int[1 << n];
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j) {
                    lineseg[(1 << i) | (1 << j)] = setids[subsets[i, j] - 1];
                }

            dp = new int[1 << n, 2];
            for (int i = 1; i < dp.GetLength(0); ++i) { dp[i, 0] = prime; dp[i, 1] = 0; }
            dp[0, 1] = 1;
            for(int i = 1; i < dp.GetLength(0); ++i)
            {
                for(int j = masks[i,0]; j <= masks[i,1]; j <<= 1)
                {
                    int seg = lineseg[j | masks[i, 0]] & masks[j | masks[i, 0], 2] & i;
                    if (dp[i, 0] > dp[i ^ seg, 0] + 1) dp[i, 0] = dp[i ^ seg, 0] + 1;
                }
            }

            sets = new List<int>();
            int mask = (1 << n) -1;
            int minH = dp[mask, 0];
            for(int i = (1 << n) - 2; i >= 0; --i)
            {
                int j1 = (~i) & mask;
                int j2 = lineseg[masks[j1, 0] | masks[j1, 1]];
                if ((((j1|j2)^j2) == 0) && (dp[i, 0] == minH - 1)) sets.Add(j1);     
            }

            sets.Sort((id1, id2) => fndcnt(id2) - fndcnt(id1));
        }

        private int[] find_solution(int n) {
            int mainset = (1 <<n) - 1;
            subsolution(mainset);
            return new int[2] { dp[mainset, 0], dp[mainset, 1] };
        }

        private int subsolution(int set) {
            if (dp[set, 1] > 0) return dp[set, 1];

            bool flag = true;
            foreach (var x in sets) flag = flag && (fndcnt(x & set) <= 2);
            if (flag)
            {
                dp[set, 1] = fac[fndcnt(set)];
                return dp[set, 1];
            }


            var subsets = new HashSet<int>();
            int max_key = 0;
            foreach(var el in sets)
            {
                int subset = el & set;
                if (subset != 0)
                    if (dp[set, 0] == dp[subset ^ set, 0] + 1)
                    {
                        if (fndcnt(max_key) < fndcnt(subset)) max_key = subset;
                        subsets.Add(subset);
                    }
            }

            foreach (var subset in subsets) {
                if(0 == ((subset | max_key)^max_key))
                {
                    dp[set, 1] = (dp[set,1] + dp[set,0] * subsolution(subset ^ set)) % prime;
                }
            }
            return dp[set, 1];
        }

        private int fndcnt(int id) {
            int cnt = 0;
            for (; id != 0; id >>= 1) cnt += 1 & id;
            return cnt;
        }

        private static int[] init_fac() {
            int[] _fac = new int[17];
            _fac[0] = _fac[1] = 1;
            for(int i = 2; i < 17; ++i) {
                int n = (i / 2) + (i % 2);
                if ((i % 2) == 0) _fac[i] = (n * (i - 1) * _fac[i - 2]) % prime;
                else _fac[i] = (n * i * _fac[i - 1]) % prime;
            }
            return _fac;
        }

        private static int[,] init_masks()
        {
            int n = 1 << 16;
            int[,] _masks = new int[n, 3];
            for (int i = 1; i < n; ++i)
            {
                for (int j = 0; (_masks[i, 0] & i) == 0; ++j) _masks[i, 0] = 1 << j;
                for (int j = n; (_masks[i, 1] & i) == 0; _masks[i, 1] = 1 << (--j));
                _masks[i, 2] = (~(_masks[i, 0] - 1)) & ((_masks[i, 1] << 1) - 1);
            }
            return _masks;
        }

        private Comparison<int[]> comp = (x1, x2) => x1[0] < x2[0] ? -1 : (x1[0] == x2[0]) && (x1[1] < x2[1]) ? -1 : 1;
        private int encodeset(int i, int setid) => setid | (1 << i);

        private readonly int[] fac = null;
        private readonly int[,] masks = null;
        private List<int> sets = null;
        private int[,] dp = null;
        private const int prime = 1000000007;


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
