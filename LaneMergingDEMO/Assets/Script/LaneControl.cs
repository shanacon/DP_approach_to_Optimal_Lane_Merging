using System;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public class LaneControl : MonoBehaviour
{
    public float TimeToInsec;
    public float NowTime;
    public int Case;
    public bool Con = false;
    UIControl uIControl;
    public float[] TimeListA1;
    public float[] TimeListB1;
    public float[] TimeListA;
    public float[] TimeListB;
    public float[] TimeListC;
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
        if(!Con)
        {
            
            TimeListA = DllLibrary.TimeListA[Case].ToArray();
            TimeListB = DllLibrary.TimeListB[Case].ToArray();
        }
        else
        {
            TimeListA1 = DllLibrary.TimeListA1[Case].ToArray();
            TimeListB1 = DllLibrary.TimeListB1[Case].ToArray();
            TimeListA = DllLibrary.TimeListA[Case].ToArray();
            TimeListB = DllLibrary.TimeListB[Case].ToArray();
            TimeListC = DllLibrary.TimeListC[Case].ToArray();
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
