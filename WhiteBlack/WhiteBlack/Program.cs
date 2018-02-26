using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WhiteBlack
{

    class Program
    {
        static void Main(string[] args)
        {
            string str = "WBWB";
            int k = 2;
            var solve = new WhiteBlack(str);
            Console.WriteLine(solve.Solve(solve.InitSeq, k));
        }
 
    }

    public class WhiteBlack
    {
        public Dictionary<int, double> Means = null;
        public int InitSeq = 0;
        int DerSeq(int seq, int pos)
        {
            int mask = (1 << pos + 1) - 1;
            return (seq & mask >> 1) | (seq & ~mask >> 1);   
        }

        public WhiteBlack(string str)
        {
            Means = new Dictionary<int, double>();
            for(var i = 0; i < str.Length; ++ i) {if (str[i] == 'W') InitSeq |= 1 << i;}
            InitSeq |= 1 << str.Length;
        }

        public double Solve(int seq, int k)
        {
            if( k == 0)
            {
                return 0;
            }
            if(Means.ContainsKey(seq))
            {
                return Means[seq];
            }
            int n = 31;
            for (; (seq & 1 << n) == 0; n --) ;
            double mean = 0.0;
            for(int i = 0, ri = n - 1; i <= ri ; ++ i, -- ri)
            {
                double p = ((ri > i) ? 2.0 : 1.0) / n;
                double ave = Solve(DerSeq(seq, i), k - 1) + (((seq & 1 << i) != 0) ? 1.0 : 0.0);
                double rave = Solve(DerSeq(seq, ri), k - 1) + (((seq & 1 << ri) != 0) ? 1.0 : 0.0);
                mean += p * ((ave < rave) ? rave : ave);
            }
            Means[seq] = mean;
            return mean;
        }
    }
}
