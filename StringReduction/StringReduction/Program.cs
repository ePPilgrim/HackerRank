using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Text.RegularExpressions;
using System.Text;
using System;

public class State{
    public enum BitMask {
        aodd = 1 << 0,
        bodd = 1 << 1,
        codd = 1 << 2,
        aeven = 1 << 3,
        beven = 1 << 4,
        ceven = 1 << 5
    };

    public BitMask X;
    public BitMask Y;

    public State(BitMask bitMask1, BitMask bitMask2){
        X = bitMask1;
        Y = bitMask2;
    }

    public BitMask findBitMask(){
        BitMask s = 0;
        s |= findOddBitMask(BitMask.aodd, BitMask.bodd, BitMask.codd, BitMask.beven | BitMask.ceven);
        s |= findOddBitMask(BitMask.bodd, BitMask.aodd, BitMask.codd, BitMask.aeven | BitMask.ceven);
        s |= findOddBitMask(BitMask.codd, BitMask.bodd, BitMask.aodd, BitMask.beven | BitMask.aeven);
        s |= findEvenBitMask(BitMask.aeven, BitMask.aodd, BitMask.beven | BitMask.ceven);
        s |= findEvenBitMask(BitMask.beven, BitMask.bodd, BitMask.aeven | BitMask.ceven);
        s |= findEvenBitMask(BitMask.ceven, BitMask.codd, BitMask.beven | BitMask.aeven);
        return s;
    }

    BitMask findOddBitMask(BitMask zodd, BitMask xodd, BitMask yodd, BitMask xyeven){
        if( (xodd & X) != 0 && (yodd & Y) != 0 ||
            (yodd & X) != 0 && (xodd & Y) != 0 ||
            (zodd & X) != 0 && ( xyeven & Y) != 0 ||
            (xyeven & X) != 0 && (zodd & Y) != 0) return zodd;
        return 0;
    } 

    BitMask findEvenBitMask(BitMask zeven, BitMask zodd, BitMask xyeven){
        if( (zeven & X) != 0 && (xyeven & Y) != 0 ||
            (xyeven & X) != 0 && (zeven & Y) != 0 ||
            (zodd & X & Y) != 0 || (zeven & X & Y) != 0)  return zeven;
        return 0;
    } 

    internal static bool IsFull(BitMask mask){
        var flag = false;
        foreach(BitMask bitval in Enum.GetValues(typeof(BitMask))){
            flag = flag &&  (mask & bitval) != 0;
        } 
        return flag;
    }

    internal static int FindMinCnt(BitMask bitMask){
        if((bitMask & (BitMask.aodd | BitMask.bodd | BitMask.codd) ) != 0) return 1;
        return 2;
    }
}

public class StringReduction{
    readonly IList<State.BitMask> sequence = null;
    readonly int initcnt = 0;
    private State.BitMask[,] subDps = new State.BitMask[100,100];
    readonly IDictionary<char, State.BitMask[]> map = new Dictionary<char, State.BitMask[]>{
        {'a', new State.BitMask[]{State.BitMask.aeven, State.BitMask.aodd}},
        {'b', new State.BitMask[]{State.BitMask.beven, State.BitMask.bodd}},
        {'c', new State.BitMask[]{State.BitMask.ceven, State.BitMask.codd}}
    };
    
    public StringReduction(string str){
        initcnt = str.Count();
        sequence = preprocessing(str);
        for(int i = 0; i < sequence.Count(); ++ i){
            subDps[i,i] = sequence[i];
        }
    }

    public int FindMinCnt(){
        dp();
        if(sequence.Count() == 1){ return initcnt;}
        int lastIndex = sequence.Count() - 1;
        return State.FindMinCnt(subDps[0, lastIndex]);
    }

    private void dp()
    {
        for(int len = 2; len <= sequence.Count(); ++ len){
            int j2 = sequence.Count() - len;
            for(int j1 = 0; j1 <= j2 ; ++ j1){
                int k2 = j1 + len - 1;
                State.BitMask dpel = 0;
                for(int k1 = j1; (k1 < k2) && (State.IsFull(dpel) == false); ++ k1 ){
                    var state = new State(subDps[j1,k1], subDps[k1 + 1, k2]);
                    dpel |= state.findBitMask();
                }
                subDps[j1,k2] = dpel;
                //Console.WriteLine($"subDps[{j1},{k2}] = {dpel}.");
            }
        }
    }

    private IList<State.BitMask> preprocessing(string str)
    {
        var seq = new List<State.BitMask>();
        for(int i = 0, cnt = 1; i < str.Count(); ++ i, ++ cnt){
            if((i == str.Count() - 1) || str[i + 1] != str[i]){
                seq.Add(map[str[i]][cnt%2]);
                cnt = 0;
                continue;
            }
        }    
        return seq;
    }
}

class Solution {

    // Complete the stringReduction function below.
    static int stringReduction(string s) {
        var str = new StringReduction(s);
        return str.FindMinCnt();
    }

    static void Main(string[] args) {
        //TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        int t = 3;//Convert.ToInt32(Console.ReadLine());
        var sss = new string[]{"cab", "bcab", "ccccc"};

        for (int tItr = 0; tItr < t; tItr++) {
            string s = sss[tItr];//Console.ReadLine();

            int result = stringReduction(s);

            //textWriter.WriteLine(result);
            Console.WriteLine(result);
        }
        //textWriter.Flush();
       // textWriter.Close();
    }
}
