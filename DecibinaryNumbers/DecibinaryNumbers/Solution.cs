using System;
using System.Collections.Generic;
using System.Linq;

namespace DecibinaryNumbers {
    class Solution {
        const int sz = 32;

        static void Main(String[] args) {
            int q = Convert.ToInt32(Console.ReadLine());
            Solution ans = new Solution();
            for (int a0 = 0; a0 < q; a0++) {
                long x = Convert.ToInt64(Console.ReadLine());
                long result = ans.decibinaryNumbers(x);
                Console.WriteLine(result);
            }
        }

        List<long[]> decamt;
        List<long> dist;
        readonly List<long> dgr10;
        readonly List<long> dgr9;

        Solution() {
            decamt = new List<long[]> { new long[sz] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };
            dist = new List<long> { 1 };
            dgr10 = new List<long> { 1 };
            dgr9 = new List<long> { 9 };
            for (int i = 0; i < sz; dgr10.Add(10 * dgr10[i++])) ;
            for (int i = 1; i < sz; ++ i) dgr9.Add((9 * (1 << i)) + dgr9[i - 1]);
        }

        long decibinaryNumbers(long x) {
            for (int i = dist.Count; x > dist.Last(); ++i) {
                int k = findPosition(i, 0);
                decamt.Add(new long[sz]);
                dist.Add(FindDecNumCnt(i, i, k) + dist[i - 1]);
                for (int j = sz - 1; j > k + 1; decamt[i][j--] = decamt[i][k + 1]) ; 
            }

            return findDecNum(x);
        }

        long FindDecNumCnt(int x, int y, int pos) {
            if (pos < 0) {
                if (y == 0) return 1;
                return 0;
            }
            if (dgr9[pos] < y) return 0;
            if (x > y) return decamt[y][pos + 1];
            long cnt = 0;
            int t = y;
            for (int i = 0; i <= 9 && t >= 0; ++i) {
                cnt += FindDecNumCnt(x, t, pos - 1);
                t -= (1 << pos);
            }
            if (x - y == 0) decamt[x][pos + 1] = cnt;
            return cnt;
        }

        private long findDecNum(long pos) {
            long ans = 0;
            int s = dist.BinarySearch(pos);
            if (s < 0) s = ~s;
            if( s > 0) pos -= dist[s - 1];
            for (int i = findPosition(s, 0); i >= 0; --i) {
                int j = 0;
                for (j = 0; decamt[s][i] < pos; ++j) {
                    pos -= decamt[s][i];
                    s -= (1 << i);
                    if (s < 0) break;
                }
                ans += j * dgr10[i];
            }
            return ans;
        }

        Func<int, int, int> findPosition = (n, k) => { for (; (n >> k + 1) != 0; ++k) ; return k; };
    }
}
