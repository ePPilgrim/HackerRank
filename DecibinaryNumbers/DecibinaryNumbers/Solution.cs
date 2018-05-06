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

        Solution() {
            decamt = new List<long[]>(5);
            decamt.Add(new long[32] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });
            decamt.Add(new long[32] { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });
            decamt.Add(new long[32] { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 });
            decamt.Add(new long[32] { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 });
            decamt.Add(new long[32] { 0, 1, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 });
            dist = new List<long> { 1, 2, 4, 6, 10 };
            dgr10 = new List<long> { 1 };
            for (int i = 0; i < 16; dgr10.Add(10 * dgr10[i++])) ;
        }

        List<long[]> decamt;
        List<long> dist;
        readonly List<long> dgr10;

        long decibinaryNumbers(long x) {
            for (int i = dist.Count; x > dist.Last(); ++i) {
                decamt.Add(FindDecNumCnt(i));
                dist.Add(decamt[i][31] + dist[i - 1]);
            }
            return findDecNum(x);
        }

        private long[] FindDecNumCnt(int x) {
            int k = findPosition(x, 0);
            var cnt = new long[sz];
            for (int i1 = 0; x - (1 << k - 1) * i1 >= 0; ++i1) {
                int x1 = x - (1 << k - 1) * i1;
                long cnt1 = 0;
                for (int i2 = 0; x1 - (1 << k - 2) * i2 >= 0; ++i2) {
                    int x2 = x1 - (1 << k - 2) * i2;
                    long cnt2 = 0;
                    if (k >= 3) {
                        for (int i3 = 0; x2 - (1 << k - 3) * i3 >= 0; ++i3) {
                            int x3 = x2 - (1 << k - 3) * i3;
                            cnt2 += decamt[x3][k - 3];
                        }
                        if (i1 + i2 == 0) cnt[k - 2] += cnt2;
                    } else if (x2 == 0) cnt2 = 1;
                    cnt1 += cnt2;
                }
                if (i1 == 0) cnt[k - 1] += cnt1;
                cnt[k] += cnt1;
            }
            cnt[k + 1] = cnt[k] + decamt[x - (1 << k)][k];
            for (int i = sz - 1; i > k + 1; cnt[i--] = cnt[k + 1]) ;
            return cnt;
        }

        private long findDecNum(long pos) {
            long ans = 0;
            int s = dist.BinarySearch(pos);
            if (s < 0) s = ~s;
            if( s > 0) pos -= dist[s - 1];
            for (int i = findPosition(s, 0); i >= 0; --i) {
                int j = 0;
                for (j = 0; decamt[s - (1 << i) * j][i] < pos; ++j) ;
                s -= (1 << i) * j;
                ans += j * dgr10[i];
            }
            return ans;
        }

        Func<int, int, int> findPosition = (n, k) => { for (; (n >> k + 1) != 0; ++k) ; return k; };
    }
}
