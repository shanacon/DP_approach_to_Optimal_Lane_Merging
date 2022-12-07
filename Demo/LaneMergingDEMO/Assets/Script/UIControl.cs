using System.Collections;
using System;
using UnityEngine;
using UnityEngine.UI;

public class UIControl : MonoBehaviour
{
    public float CarSpeed;
    public float SpeedRate = 1.0f;
    float timer = 0.0f;
    bool TimeStart;
    Text TimeText;
    Text CompleteText1;
    Text CompleteText2;
    Text SpeedText;
    LaneControl[] laneControl;
    // Start is called before the first frame update
    void Awake()
    {
        TimeText = transform.Find("Time").GetComponent<Text>();
        CompleteText1 = transform.Find("CompleteTime1").GetComponent<Text>();
        CompleteText2 = transform.Find("CompleteTime2").GetComponent<Text>();
        SpeedText = transform.Find("SpeedRateText").GetComponent<Text>();
        TimeText.text = "0.0";
        transform.Find("StartBtn").GetComponent<Button>().onClick.AddListener(BtnEvent);
        transform.Find("FastBtn").GetComponent<Button>().onClick.AddListener(delegate { SetSpeedRate(true); });
        transform.Find("SlowBtn").GetComponent<Button>().onClick.AddListener(delegate { SetSpeedRate(false); });
        GameObject[] LaneObject = GameObject.FindGameObjectsWithTag("LaneControl");
        laneControl = new LaneControl[LaneObject.Length];
        for(int i = 0; i < laneControl.Length; i++)
            laneControl[i] = LaneObject[i].GetComponent<LaneControl>();

    }
    void Start()
    {
        
    }
    // Update is called once per frame
    void Update()
    {
        SpeedText.text = SpeedRate.ToString("0.0");
        if(TimeStart)
        {
            timer += Time.deltaTime * SpeedRate;
            TimeText.text = timer.ToString("0.00");
            foreach(LaneControl LC in laneControl)
                LC.NowTime = timer;
        }
        
    }
    public void BtnEvent()
    {
        TimeStart = true;
        foreach(GameObject lane in GameObject.FindGameObjectsWithTag("StartPoint"))
            lane.GetComponent<CreateCar>().enabled = true;
    }
    public void SetInitialTime(float t)
    {
        timer = t;
        TimeText.text = t.ToString("0.00");
    }
    public void SetCompleteTime(int Case)
    {
        if(Case == 0)
            CompleteText1.text = timer.ToString("0.00");
        if(Case == 1)
            CompleteText2.text = timer.ToString("0.00");
    }
    public void SetSpeedRate(bool Faster)
    {
        if(Faster)
            SpeedRate = Math.Min(SpeedRate * 2, 8.0f);
        else if(!Faster)
            SpeedRate = Math.Max(SpeedRate / 2, 1.0f);
    }
}
