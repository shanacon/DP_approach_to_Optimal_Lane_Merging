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
    private TextAsset Result;
    UIControl uIControl;
    float[] TimeListA;
    float[] TimeListB;
    bool end = false;
    
    // Start is called before the first frame update
    void Start()
    {
        // calculate TimeToInsec
        Transform StartPoint = GameObject.FindGameObjectWithTag("StartPoint").transform;
        Transform EndPoint = GameObject.FindGameObjectWithTag("EndPoint").transform;
        float CarSpeed = GameObject.FindGameObjectWithTag("UIControl").GetComponent<UIControl>().CarSpeed;
        TimeToInsec = (float)(StartPoint.position.x - EndPoint.position.x) / CarSpeed;
        uIControl = GameObject.FindGameObjectWithTag("UIControl").GetComponent<UIControl>();
        uIControl.SetInitialTime(TimeToInsec);
        // handle result file
        Result = Resources.Load<TextAsset>(FileNameInResources);
        string[] lineData = Result.text.Split('\n');
        TimeListA = Array.ConvertAll(lineData[3].Remove(lineData[3].Length - 2).Split(' '), s => float.Parse(s));
        TimeListB = Array.ConvertAll(lineData[4].Remove(lineData[4].Length - 2).Split(' '), s => float.Parse(s));
    }

    // Update is called once per frame
    void Update()
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
        if(NowTime > Math.Max(TimeListA.Last(), TimeListB.Last()) && !end)
        {
            end = true;
            uIControl.SetCompleteTime(Case);
        } 
    }
}
