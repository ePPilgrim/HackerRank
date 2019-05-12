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

public class StringReduction{

    enum State : byte{
        aodd = 1,
        bodd = 1 << 1,
        codd = 1 << 2,
        aeven = 1 << 3,
        beven = 1 << 4,
        ceven = 1 << 5
    };
    readonly string sequence = null;
    
    public StringReduction(string str){
        sequence = preprocessing(str);
    }

    private string preprocessing(string str)
    {
        throw new NotImplementedException();
    }

    State findstate(State s1, State s2){
        State s = 0;
        bool b = (State.bodd & s1) != 0 && (State.codd & s2) != 0 ||
                    (State.codd & s1) != 0 && (State.bodd & s2) != 0 ||
                    (State.aodd & s1) != 0 && (State.beven | State.ceven & s2) != 0 ||
                    (State.beven | State.ceven & s1) != 0 && (State.aodd & s2) != 0;

        return s;
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
