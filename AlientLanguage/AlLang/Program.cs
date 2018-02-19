using System;
using System.Collections.Generic;

namespace AlLang
{
    class Program
    {
        const ulong prime = 100000007UL;
        static void Main(string[] args)
        {
            for (var i = 0; i < Convert.ToInt32(Console.ReadLine()) ; ++ i)
            {
                string[] str = Console.ReadLine().Split(' ');
                ulong n = Convert.ToUInt64(str[0]);
                ulong m = Convert.ToUInt64(str[1]);
                Console.WriteLine(Solve(n, m));
            } 
        }

        static ulong Solve(ulong n, ulong m)
        {
            ulong[,] arr = new ulong[2, n + 1];
            var k = n % 2 + n / 2;
            var fac = new List<ulong> { k };
            for (var i = 1UL; i <= n / 2; ++i) arr[0, i] = (2 * i <= n / 2) ? k : n + 1 - 2 * i;
            k = n / 2;
            for ( ulong i = 0UL; k > 0 ;k /=2, i++)
            {
                ulong val = 0, q = i % 2;
                for (ulong j = k; j > 1; -- j)
                {
                    val = ( val + arr[q, j]) % prime;
                    arr[1 - q, j / 2] = val % prime;
                }
                fac.Add((arr[0, 1] + arr[1, 1]) % prime);
                arr[q, 1] = 0;
            }
            ulong[] res = new ulong[fac.Count];
            res[0] = 1;
            for( ulong i = 0; i < m; ++ i)
            {
                ulong ans = 0;
                for (int j = 0; j < res.Length; ++j) ans += (res[j] * fac[j]) % prime;
                Array.ConstrainedCopy(res, 0, res, 1, res.Length - 1);
                res[0] = ans % prime;
            }
            return res[0];
        }

    }
}
