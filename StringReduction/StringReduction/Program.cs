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
        aodd = 1,
        bodd = 1 << 1,
        codd = 1 << 2,
        aeven = 1 << 3,
        beven = 1 << 4,
        ceven = 1 << 5
    };

    public BitMask X;
    public BitMask Y;

    BitMask findBitMask(){
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
}

public class StringReduction{


    readonly string sequence = null;
    
    public StringReduction(string str){
        sequence = preprocessing(str);
    }

    private string preprocessing(string str)
    {
        throw new NotImplementedException();
    }


}

class Solution {

    // Complete the stringReduction function below.
    static int stringReduction(string s) {
        throw new NotImplementedException();
    }

    static void Main(string[] args) {
        TextWriter textWriter = new StreamWriter(@System.Environment.GetEnvironmentVariable("OUTPUT_PATH"), true);

        int t = Convert.ToInt32(Console.ReadLine());

        for (int tItr = 0; tItr < t; tItr++) {
            string s = Console.ReadLine();

            int result = stringReduction(s);

            textWriter.WriteLine(result);
        }

        textWriter.Flush();
        textWriter.Close();
    }
}
