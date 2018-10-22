using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace PointsOnPlane
{
    class Program
    {
        static int[] pointsInPlane(int[][] coordinates)
        {
            //Comparison<int[]> comp = (x1, x2) => x1[0] < x2[0] ? -1 : (x1[0] == x2[0]) && (x1[1] < x2[1]) ? -1 : 1;
            int comp(int[] x1, int[] x2) => x1[0] < x2[0] ? -1 : (x1[0] == x2[0]) && (x1[1] < x2[1]) ? -1 : 1;
            Array.Sort(coordinates, Comparer<int[]>.Create(comp));
            return new int[2];
        }

        static void Main(string[] args)
        {
        }
    }
}
