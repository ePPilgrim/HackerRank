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
            //            29 28
            //WBWBWBWBWBWBWBWBWBWBWBWBWBWBW
            //14.9975369458

            //4 2
            //WWBW
            //1.8333333333
            string str = "WBWBWBWBWBWBWBWBWBWBWBWBWBWBW";
            int k = 28;
       
            var solve = new WhiteBlack(str);
            Console.WriteLine(String.Format("{0:F10}", solve.Solve(solve.InitSeq, k)));
            Console.WriteLine(solve.Means.Count);
        }
 
    }

    public class WhiteBlack
    {
        public Dictionary<uint, double> Means = null;
        public uint InitSeq = 0;
        uint DerSeq(uint seq, int pos)
        {
            uint mask = (1u << pos + 1) - 1;
            return (seq & mask >> 1) | (~mask & seq) >> 1;   
        }

        public WhiteBlack(string str)
        {
            Means = new Dictionary<uint, double>();
            for(var i = 0; i < str.Length; ++ i) {if (str[i] == 'W') InitSeq |= 1u << i;}
            InitSeq |= 1u << str.Length;
        }

        public double Solve(uint seq, int k)
        {
            if (k == 0) return 0;
            if(Means.ContainsKey(seq)) return Means[seq];
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
