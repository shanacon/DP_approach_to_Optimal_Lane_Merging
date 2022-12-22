using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;
using UnityEngine.UI;

public class UIControl : MonoBehaviour
{
    public float CarSpeed;
    public float SpeedRate = 1.0f;
    public float Tf = 2.0f;
    private int NowScene = 0; // 0 for two lane merging , 1 for con lane merging
    float timer = 0.0f;
    bool TimeStart;
    Text TimeText;
    Text CompleteText1;
    Text CompleteText2;
    Text SpeedText;
    Text DPL;
    Text DPT;
    Text FIFOL;
    Text FIFOT;
    LaneControl[] laneControl;
    InputField LambdaInput;
    InputField CaseLenInput;
    Toggle TwoToggle;
    Toggle ConToggle;
    GameObject TwoLane;
    GameObject ConLane;
    // Start is called before the first frame update
    void Awake()
    {
        TimeText = transform.Find("Time").GetComponent<Text>();
        CompleteText1 = transform.Find("CompleteTime1").GetComponent<Text>();
        CompleteText2 = transform.Find("CompleteTime2").GetComponent<Text>();
        SpeedText = transform.Find("SpeedRateText").GetComponent<Text>();
        DPL = transform.Find("DPL").GetComponent<Text>();
        DPT = transform.Find("DPT").GetComponent<Text>();
        FIFOL = transform.Find("FIFOL").GetComponent<Text>();
        FIFOT = transform.Find("FIFOT").GetComponent<Text>();
        //
        LambdaInput = transform.Find("Lambda").GetComponent<InputField>();
        CaseLenInput = transform.Find("CaseLen").GetComponent<InputField>();
        TwoToggle = transform.Find("TwoCheck").GetComponent<Toggle>();
        ConToggle = transform.Find("ConCheck").GetComponent<Toggle>();
        TwoToggle.onValueChanged.AddListener(delegate { CheckToggle(0); });
        ConToggle.onValueChanged.AddListener(delegate { CheckToggle(1); });
        //
        TimeText.text = "0.0";
        transform.Find("StartBtn").GetComponent<Button>().onClick.AddListener(StartEvent);
        transform.Find("ChangeSceneBtn").GetComponent<Button>().onClick.AddListener(ChangeScence);
        transform.Find("FastBtn").GetComponent<Button>().onClick.AddListener(delegate { SetSpeedRate(true); });
        transform.Find("SlowBtn").GetComponent<Button>().onClick.AddListener(delegate { SetSpeedRate(false); });
        transform.Find("RandomBtn").GetComponent<Button>().onClick.AddListener(RandomData);
        TwoLane = GameObject.Find("TwoLane");
        ConLane = GameObject.Find("ConLane");
        GameObject[] LaneObject = GameObject.FindGameObjectsWithTag("LaneControl");
        laneControl = new LaneControl[LaneObject.Length];
        for(int i = 0; i < laneControl.Length; i++)
            laneControl[i] = LaneObject[i].GetComponent<LaneControl>();
        TwoLane.SetActive(false);
        ConLane.SetActive(false);
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
    public void StartEvent()
    {
        if(TwoToggle.isOn)
        {
            TwoLane.SetActive(true);
        }
        else if(ConToggle.isOn)
        {
            ConLane.SetActive(true);
        }
        LambdaInput.gameObject.SetActive(false);
        CaseLenInput.gameObject.SetActive(false);
        TwoToggle.gameObject.SetActive(false);
        ConToggle.gameObject.SetActive(false);
        DPL.gameObject.SetActive(false);
        DPT.gameObject.SetActive(false);
        FIFOL.gameObject.SetActive(false);
        FIFOT.gameObject.SetActive(false);
        transform.Find("RandomBtn").gameObject.SetActive(false);
        transform.Find("AllInputText").gameObject.SetActive(false);
        TimeStart = true;
        foreach(GameObject lane in GameObject.FindGameObjectsWithTag("StartPoint"))
        {
            lane.GetComponent<CreateCar>().enabled = true;
            lane.GetComponent<CreateCar>().Tf = Tf;
            lane.GetComponent<CreateCar>().InitialTime = timer;
            lane.GetComponent<CreateCar>().speed = CarSpeed;
            lane.GetComponent<CreateCar>().SetInitialTime();
        }
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
    private void ChangeScence()
    {
        if(NowScene == 0)
        {
            TwoLane.SetActive(false);
            ConLane.SetActive(true);
            NowScene = 1;
        }
        else if(NowScene == 1)
        {
            TwoLane.SetActive(true);
            ConLane.SetActive(false);
            NowScene = 0;
        } 
    }
    private void RandomData()
    {
        DllLibrary.ClearAll();
        double labda =  Convert.ToDouble(LambdaInput.text);
        int Caselen =  Convert.ToInt32(CaseLenInput.text);
        if(TwoToggle.isOn)
        {
            DllLibrary.SetUpRandom(labda, Caselen, 2);
            DllLibrary.DoDP();
            SetCalData(0);
            DPL.text = DllLibrary.GetFinalTime().ToString();
            DPT.text = DllLibrary.GetTDelay().ToString();
            ///
            DllLibrary.DoFIFO();
            SetCalData(1);
            FIFOL.text = DllLibrary.GetFinalTime().ToString();
            FIFOT.text = DllLibrary.GetTDelay().ToString();
        }  
        else
        {
            DllLibrary.SetUpRandom(labda, Caselen, 3);
            DllLibrary.DoConDP();
            SetCalData(0);
            DPL.text = DllLibrary.GetFinalTime().ToString();
            DPT.text = DllLibrary.GetTDelay().ToString();
            ///
            DllLibrary.DoConFIFO();
            SetCalData(1);
            FIFOL.text = DllLibrary.GetFinalTime().ToString();
            FIFOT.text = DllLibrary.GetTDelay().ToString();
        }
            
    }
    private void CheckToggle(int i)
    {
        if(i == 0 && TwoToggle.isOn)
            ConToggle.isOn = false;
        else if (i == 1 && ConToggle.isOn)
            TwoToggle.isOn = false;
    }
    private void SetCalData(int index)
    {
        DllLibrary.TimeListA[index] = new List<float>();
        DllLibrary.TimeListB[index] = new List<float>();
        DllLibrary.TimeListC[index] = new List<float>();
        DllLibrary.TimeListA1[index] = new List<float>();
        DllLibrary.TimeListB1[index] = new List<float>();
        DllLibrary.Atimes[index] = new List<float>();
        DllLibrary.Btimes[index] = new List<float>();
        DllLibrary.Ctimes[index] = new List<float>();
        int s = -1;
        float[] arr = new float[1000];
        ////
        s = DllLibrary.GetTimeAns(1,arr);
        for(int i = 0;i < s;i++)
            DllLibrary.TimeListA[index].Add((float)arr[i]);
        s = DllLibrary.GetTimeAns(2,arr);
        for(int i = 0;i < s;i++)
            DllLibrary.TimeListB[index].Add((float)arr[i]);
        s = DllLibrary.GetTimeAns(3,arr);
        for(int i = 0;i < s;i++)
            DllLibrary.TimeListC[index].Add((float)arr[i]);
        ////
        s = DllLibrary.GetTimeOneAns(1,arr);
        for(int i = 0;i < s;i++)
            DllLibrary.TimeListA1[index].Add((float)arr[i]);
        s = DllLibrary.GetTimeOneAns(2,arr);
        for(int i = 0;i < s;i++)
            DllLibrary.TimeListB1[index].Add((float)arr[i]);
        ////
        int[] IntArr = new int[1000];
        s = DllLibrary.GetRealTime(1,IntArr);
        for(int i = 0;i < s;i++)
            DllLibrary.Atimes[index].Add((float)IntArr[i]);
        s = DllLibrary.GetRealTime(2,IntArr);
        for(int i = 0;i < s;i++)
            DllLibrary.Btimes[index].Add((float)IntArr[i]);
        s = DllLibrary.GetRealTime(3,IntArr);
        for(int i = 0;i < s;i++)
            DllLibrary.Ctimes[index].Add((float)IntArr[i]);
    }
}
