using System;
using System.Runtime.InteropServices;
using UnityEngine;
using System.Collections.Generic;

static public class DllLibrary
{
    [DllImport("LaneMergeDll.dll")]
    public static extern void SetUpRandom(double lamba, int CaseLen, int CaseNum);
    [DllImport("LaneMergeDll.dll")]
    public static extern int GetTimeList(int LaneNum, int[] arr);
    [DllImport("LaneMergeDll.dll")]
    public static extern int GetRealTime(int LaneNum, int[] RealTime);
    [DllImport("LaneMergeDll.dll")]
    public static extern void DoDP();
    [DllImport("LaneMergeDll.dll")]
    public static extern void DoConDP();
    [DllImport("LaneMergeDll.dll")]
    public static extern void DoFIFO();
    [DllImport("LaneMergeDll.dll")]
    public static extern void DoConFIFO();
    [DllImport("LaneMergeDll.dll")]
    public static extern int GetTimeOneAns(int LaneNum, int[] TimeList);
    [DllImport("LaneMergeDll.dll")]
    public static extern int GetTimeAns(int LaneNum, int[] TimeList);
    [DllImport("LaneMergeDll.dll")]
    public static extern int GetFinalTime();
    [DllImport("LaneMergeDll.dll")]
    public static extern double GetTDelay();
    [DllImport("LaneMergeDll.dll")]
    public static extern void ClearAll();
    [DllImport("LaneMergeDll.dll")]
    public static extern void Test(int[] s);

    public static List<float>[] TimeListA = new List<float>[2];
    public static List<float>[] TimeListB = new List<float>[2];
    public static List<float>[] TimeListC = new List<float>[2];
    public static List<float>[] TimeListA1 = new List<float>[2];
    public static List<float>[] TimeListB1 = new List<float>[2];
    public static List<float>[] Atimes = new List<float>[2];
    public static List<float>[] Btimes = new List<float>[2];
    public static List<float>[] Ctimes = new List<float>[2];
}
