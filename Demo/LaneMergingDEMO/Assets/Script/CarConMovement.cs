using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CarConMovement : MonoBehaviour
{
    public int lane;
    public float GoTime;
    public float GoTime2;
    public float NowTime;
    public string CarID;
    public bool GoSign1 = false;
    public bool GoSign2 = false;
    public float Tf;
    private float Sequal = 0.1f; // time distance before merge
    private float speed1;
    private float speed2;
    private float WequalDistance;
    private bool moving = false;
    private bool turning = false;
    float SpeedRate;
    float HalfCarX;
    float Y1Speed;
    float Y2Speed;
    float TurnAngle1 = 14.5f;
    float TurnAngle2 = -10f;
    int phase = 0; // 0 for initial, 1 for transfer, 2 for end
    UIControl uIControl;
    Transform RayShooter;
    SpriteRenderer spriteRenderer;
    Transform EndPoint1;
    Transform CompletePoint1;
    Transform EndPoint2;
    Transform CompletePoint2;
    void Start()
    {
        spriteRenderer = GetComponent<SpriteRenderer>();
        uIControl = GameObject.FindGameObjectWithTag("UIControl").GetComponent<UIControl>();
        speed1 = uIControl.CarSpeed;
        SpeedRate = uIControl.SpeedRate;
        EndPoint1 = transform.parent.Find("EndPoint1").transform;
        CompletePoint1 = transform.parent.Find("CompletePoint1").transform;
        EndPoint2 = transform.parent.Find("EndPoint2").transform;
        CompletePoint2 = transform.parent.Find("CompletePoint2").transform;
        RayShooter = gameObject.transform.Find("RayShooter");
        HalfCarX = (float)(GetComponent<BoxCollider2D>().size.x) / 2.0f;
        WequalDistance = speed1 * Sequal;
        if(lane == 0)
        {
            spriteRenderer.color = Color.blue;
            float CrossTime2 = (float)((CompletePoint2.position.x - EndPoint2.position.x) / speed1);
            Y2Speed = -(float)((CompletePoint1.position.y - CompletePoint2.position.y) / CrossTime2);
            speed2 = (float)((EndPoint2.position.x - EndPoint1.position.x) / Tf);
            float CrossTime = (float)((CompletePoint1.position.x - EndPoint1.position.x) / speed2);
            Y1Speed = -(float)((transform.position.y - CompletePoint1.position.y) / CrossTime);
            TurnAngle1 = -TurnAngle1;
        } 
        if(lane == 1)
        {
            spriteRenderer.color = Color.yellow;
            float CrossTime2 = (float)((CompletePoint2.position.x - EndPoint2.position.x) / speed1);
            Y2Speed = -(float)((CompletePoint1.position.y - CompletePoint2.position.y) / CrossTime2);
            speed2 = (float)((EndPoint2.position.x - EndPoint1.position.x) / Tf);
            float CrossTime = (float)((CompletePoint1.position.x - EndPoint1.position.x) / speed2);
            Y1Speed = -(float)((transform.position.y - CompletePoint1.position.y) / CrossTime);
        }
        if(lane == 2)
        {
            spriteRenderer.color = Color.red;
            gameObject.GetComponent<BoxCollider2D>().enabled = false;
            float CrossTime2 = (float)((CompletePoint2.position.x - EndPoint2.position.x) / speed1);
            Y2Speed = -(float)((transform.position.y - CompletePoint2.position.y) / CrossTime2);
            phase = 1;
            TurnAngle2 = -TurnAngle2;
        }
        if(lane == 3)
            spriteRenderer.color = Color.green;
    }

    // Update is called once per frame
    void Update()
    {
        SpeedRate = uIControl.SpeedRate;
        // Destroy Handle
        if(transform.position.x > 13.5f)
            Destroy(this.gameObject);
        // Do RayCast
        float FrontDistance = WequalDistance + 10.0f;
        RaycastHit2D hit = Physics2D.Raycast(RayShooter.position, Vector2.right);
        if (hit.collider != null)
            FrontDistance = hit.point.x - RayShooter.position.x;
        // check stop
        if(FrontDistance < WequalDistance)
            moving = false;
        else if (phase == 0 && transform.position.x >= EndPoint1.position.x && !GoSign1)
            moving = false;
        else if (phase == 1 && transform.position.x >= EndPoint2.position.x && !GoSign2)
            moving = false;
        else
            moving = true;
        // cross intersection
        if(phase == 0 && transform.position.x > CompletePoint1.position.x)
        {
            turning = false;
            transform.eulerAngles = new Vector3(0, 0, 0);
            transform.position = new Vector3(transform.position.x , CompletePoint1.transform.position.y , transform.position.z);
            phase = 1;
        }
        else if(phase == 1 && transform.position.x > CompletePoint2.position.x)
        {
            turning = false;
            transform.eulerAngles = new Vector3(0, 0, 0);
            transform.position = new Vector3(transform.position.x , CompletePoint2.transform.position.y , transform.position.z);
            phase = 2;
        }
        else if(phase == 0 && GoSign1)
            turning = true;
        else if(phase == 1 && GoSign2)
            turning = true;
        // move and turn
        if(moving)
        {
            if((phase == 0 && GoSign1) || (phase == 1 && !GoSign2 && lane != 2))
                transform.position = new Vector3(transform.position.x + speed2 * Time.deltaTime * SpeedRate, transform.position.y, transform.position.z);
            else
                transform.position = new Vector3(transform.position.x + speed1 * Time.deltaTime * SpeedRate, transform.position.y, transform.position.z);
        }
            
        if(turning)
        {
            if(phase == 0)
            {
                transform.eulerAngles = new Vector3(0, 0, TurnAngle1);
                transform.position = new Vector3(transform.position.x , transform.position.y + Y1Speed * Time.deltaTime * SpeedRate, transform.position.z);
            }
            if(phase == 1)
            {
                transform.eulerAngles = new Vector3(0, 0, TurnAngle2);
                transform.position = new Vector3(transform.position.x , transform.position.y + Y2Speed * Time.deltaTime * SpeedRate, transform.position.z);
            }  
        } 
    }
}
