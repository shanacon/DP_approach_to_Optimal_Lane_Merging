using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;

public class CreateCar : MonoBehaviour
{
    public GameObject car;
    public string FileNameInResources;
    public int LaneNum;
    private TextAsset Result;
    float[] Times;
    int Index = 1;
    float timer = 0.0f;
    int count = 0;
    void Start()
    {
        Result = Resources.Load<TextAsset>(FileNameInResources);
        string[] lineData = Result.text.Split('\n');
        Times = Array.ConvertAll(lineData[LaneNum].Remove(lineData[LaneNum].Length - 2).Split(' '), s => float.Parse(s));
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
            GameObject NewCar =  Instantiate(car, pos, car.transform.rotation);
            NewCar.transform.parent = this.gameObject.transform.parent;
            NewCar.GetComponent<CarMovement>().lane = LaneNum;
            NewCar.GetComponent<CarMovement>().CarID = LaneNum.ToString() + "@" + (Index - 1).ToString();
            Index++;
        }
    }
}