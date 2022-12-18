using System;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public class LaneControl : MonoBehaviour
{
    public string FileNameInResources;
    public float TimeToInsec;
    public float NowTime;
    public int Case;
    public bool Con = false;
    private TextAsset Result;
    UIControl uIControl;
    float[] TimeListA1;
    float[] TimeListB1;
    float[] TimeListA;
    float[] TimeListB;
    float[] TimeListC;
    bool end = false;
    
    // Start is called before the first frame update
    void Start()
    {
        // calculate TimeToInsec
        Transform StartPoint = transform.Find("StartPoint1").transform;
        Transform EndPoint = transform.Find("EndPoint1").transform;
        float CarSpeed = GameObject.FindGameObjectWithTag("UIControl").GetComponent<UIControl>().CarSpeed;
        TimeToInsec = (float)(StartPoint.position.x - EndPoint.position.x) / CarSpeed;
        uIControl = GameObject.FindGameObjectWithTag("UIControl").GetComponent<UIControl>();
        uIControl.SetInitialTime(TimeToInsec);
        // handle result file
        Result = Resources.Load<TextAsset>(FileNameInResources);
        string[] lineData = Result.text.Split('\n');
        if(!Con)
        {
            TimeListA = Array.ConvertAll(lineData[3].Remove(lineData[3].Length - 2).Split(' '), s => float.Parse(s));
            TimeListB = Array.ConvertAll(lineData[4].Remove(lineData[4].Length - 2).Split(' '), s => float.Parse(s));
        }
        else
        {
            TimeListA1 = Array.ConvertAll(lineData[5].Remove(lineData[5].Length - 2).Split(' '), s => float.Parse(s));
            TimeListB1 = Array.ConvertAll(lineData[6].Remove(lineData[6].Length - 2).Split(' '), s => float.Parse(s));
            TimeListA = Array.ConvertAll(lineData[7].Remove(lineData[7].Length - 2).Split(' '), s => float.Parse(s));
            TimeListB = Array.ConvertAll(lineData[8].Remove(lineData[8].Length - 2).Split(' '), s => float.Parse(s));
            TimeListC = Array.ConvertAll(lineData[9].Remove(lineData[9].Length - 2).Split(' '), s => float.Parse(s));
        }
    }

    // Update is called once per frame
    void Update()
    {
        if(!Con)
        {
            CarMovement[] carMovement = GetComponentsInChildren<CarMovement>();
            foreach(CarMovement CM in carMovement)
            {
                string[] CarID = CM.CarID.Split('@');
                CM.NowTime = NowTime;
                if(CarID[0] == "0")
                    CM.GoTime = TimeListA[int.Parse(CarID[1])];
                if(CarID[0] == "1")
                    CM.GoTime = TimeListB[int.Parse(CarID[1])];
            }
            if(!Con)
            {
                if(NowTime > Math.Max(TimeListA.Last(), TimeListB.Last()) && !end)
                {
                    end = true;
                    uIControl.SetCompleteTime(Case);
                } 
            }
            else
            {
                if(NowTime > Math.Max(TimeListA.Last(), Math.Max(TimeListB.Last(), TimeListC.Last())) && !end)
                {
                    end = true;
                    uIControl.SetCompleteTime(Case);
                } 
            }
        }
        else
        {
            CarConMovement[] carMovement = GetComponentsInChildren<CarConMovement>();
            foreach(CarConMovement CM in carMovement)
            {
                string[] CarID = CM.CarID.Split('@');
                CM.NowTime = NowTime;
                if(CarID[0] == "0")
                {
                    CM.GoTime = TimeListA1[int.Parse(CarID[1])];
                    if(NowTime > TimeListA1[int.Parse(CarID[1])])
                        CM.GoSign1 = true;
                    if(NowTime > TimeListA[int.Parse(CarID[1])])
                        CM.GoSign2 = true;
                }
                if(CarID[0] == "1")
                {
                    CM.GoTime = TimeListB1[int.Parse(CarID[1])];
                    if(NowTime > TimeListB1[int.Parse(CarID[1])])
                        CM.GoSign1 = true;
                    if(NowTime > TimeListB[int.Parse(CarID[1])])
                        CM.GoSign2 = true;
                }      
                if(CarID[0] == "2")
                {
                    CM.GoTime = TimeListC[int.Parse(CarID[1])];
                    if(NowTime > TimeListC[int.Parse(CarID[1])])
                        CM.GoSign2 = true;
                } 
            }
            if(NowTime > Math.Max(TimeListA.Last(), TimeListB.Last()) && !end)
            {
                end = true;
                uIControl.SetCompleteTime(Case);
            } 
        }
    }
}
