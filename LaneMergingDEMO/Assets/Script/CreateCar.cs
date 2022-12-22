using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;

public class CreateCar : MonoBehaviour
{
    public GameObject car;
    public int DPorFIFO;
    public int LaneNum;
    public bool Con = false;
    public float Tf;
    public float InitialTime;
    public float speed; //Initial Speed
    public bool SmallerCar;
    private TextAsset Result;
    public float[] Times;
    int Index = 1;
    float timer = 0.0f;
    int count = 0;
    void Start()
    {
        if(LaneNum == 0)
            Times = DllLibrary.Atimes[DPorFIFO].ToArray();
        if(LaneNum == 1)
            Times = DllLibrary.Btimes[DPorFIFO].ToArray();
        if(LaneNum == 2)
            Times = DllLibrary.Ctimes[DPorFIFO].ToArray();
    }
    void Update()
    {
        timer += Time.deltaTime;
        if(Index >= Times.Length)
            Destroy(this.gameObject);
        else if(timer >= Times[Index])
        {
            float dis = 10.0f;
            Vector3 pos = transform.position;
            RaycastHit2D hit = Physics2D.Raycast(transform.position, Vector2.right);
            if (hit.collider != null)
                dis = hit.point.x - transform.position.x;
            if(dis < 1.0f)
            {
                count++;
                pos = new Vector3(transform.position.x - (0.1f * count), transform.position.y, transform.position.z);
            } 
            else
                count = 0;
            pos.z-=(0.1f * Index);
            GameObject NewCar =  Instantiate(car, pos, car.transform.rotation);
            NewCar.transform.parent = this.gameObject.transform.parent;
            if(!Con)
            {
                NewCar.GetComponent<CarMovement>().lane = LaneNum;
                NewCar.GetComponent<CarMovement>().CarID = LaneNum.ToString() + "@" + (Index - 1).ToString();
            }
            else
            {
                NewCar.GetComponent<CarConMovement>().lane = LaneNum;
                NewCar.GetComponent<CarConMovement>().CarID = LaneNum.ToString() + "@" + (Index - 1).ToString();
                NewCar.GetComponent<CarConMovement>().Tf = Tf;
            }
            if(SmallerCar)
            {
                BoxCollider2D collider2D = NewCar.GetComponent<BoxCollider2D>();
                collider2D.offset = new Vector2(collider2D.offset.x + 0.35f, 0f);
                collider2D.size = new Vector2(collider2D.size.x * 0.22f, 1f);
            }
            Index++;
        }
    }
    public void SetInitialTime()
    {
        if(LaneNum == 2)
        {
            Transform StartPoint3 = transform.parent.Find("StartPoint3");
            Transform EndPoint2 = transform.parent.Find("EndPoint2");
            float ToSecTime = (float)((EndPoint2.position.x - StartPoint3.position.x)/speed);
            timer-= ToSecTime;
        }
    }
}
