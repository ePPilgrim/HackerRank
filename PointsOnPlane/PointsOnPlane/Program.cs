using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace PointsOnPlane
{
    class Program
    {
        static int[] pointsInPlane(int[][] c)
        {
            Comparison<int[]> comp = (x1, x2) => x1[0] < x2[0] ? -1 : (x1[0] == x2[0]) && (x1[1] < x2[1]) ? -1 : 1;
            Array.Sort(c, Comparer<int[]>.Create(comp));
            int n = c.Length;
            int[,] dp = new int[1 << n, 2];
            int[,] sets = new int[n, n];
            List<int> setids = new List<int> { 0 };
            Func<int[], int[], bool> iscolinear = (p1, p2) => (p1[0] * p2[1] - p1[1] * p2[0]) == 0;
            int encodeset(int i, int setid) => setid | (1 << i);
            for(int i = 0; i < n; ++i)
            {
                setids.Add(encodeset(i, 0));
                for(int j = i + 1; j < n; ++ j)
                {
                    for(int k = 0;k < j; ++ k)
                    {

                    }
                }

            }



            return new int[2];
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Number = {0}",1<<3);
            int hhh = 9;
        }
    }
}
