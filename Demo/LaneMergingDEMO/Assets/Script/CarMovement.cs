using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CarMovement : MonoBehaviour
{
    public int lane;
    public float GoTime;
    public float NowTime;
    public string CarID;
    private float Sequal = 0.1f; // time distance before merge
    private float speed;
    private float WequalDistance;
    private bool moving = false;
    private bool turning = false;
    float HalfCarX;
    float YSpeed;
    float TurnAngle = 361.0f;
    Transform RayShooter;
    SpriteRenderer spriteRenderer;
    Transform EndPoint;
    Transform CompletePoint;
    void Start()
    {
        spriteRenderer = GetComponent<SpriteRenderer>();
        if(lane == 0)
            spriteRenderer.color = Color.blue;
        if(lane == 1)
            spriteRenderer.color = Color.yellow;
        if(lane == 2)
            spriteRenderer.color = Color.red;
        if(lane == 3)
            spriteRenderer.color = Color.green;
        speed = GameObject.FindGameObjectWithTag("UIControl").GetComponent<UIControl>().CarSpeed;
        EndPoint = GameObject.FindGameObjectWithTag("EndPoint").transform;
        CompletePoint = transform.parent.Find("CompletePoint");
        RayShooter = gameObject.transform.Find("RayShooter");
        HalfCarX = (float)(GetComponent<BoxCollider2D>().size.x) / 2.0f;
        WequalDistance = speed * Sequal;
        float CrossTime = (float)((CompletePoint.position.x - EndPoint.position.x) / speed);
        YSpeed = -(float)((transform.position.y - CompletePoint.position.y) / CrossTime);
    }

    // Update is called once per frame
    void Update()
    {
        // Destroy Handle
        if(transform.position.x > 13.5f)
            Destroy(this.gameObject);
        // Do RayCast
        float FrontDistance = WequalDistance + 10.0f;
        RaycastHit2D hit = Physics2D.Raycast(RayShooter.position, Vector2.right);
        if (hit.collider != null)
            FrontDistance = hit.point.x - RayShooter.position.x;
        // check stop
        if(NowTime >= GoTime)
            moving = true;
        else if(EndPoint.position.x < transform.position.x + HalfCarX || FrontDistance < WequalDistance)
            moving = false;
        else
            moving = true;
        // cross intersection
        if(NowTime >= GoTime && transform.position.x < CompletePoint.position.x)
        {
            if(TurnAngle == 361.0f)
            {
                if(transform.position.y > CompletePoint.transform.position.y)
                    TurnAngle = -90 + Vector3.Angle(CompletePoint.transform.position, transform.right);
                else
                    TurnAngle = 90 -Vector3.Angle(CompletePoint.transform.position, transform.right);
            }
            turning = true;
        } 
        else if(turning && transform.position.x > CompletePoint.position.x)
        {
            turning = false;
            transform.eulerAngles = new Vector3(0, 0, 0);
            transform.position = new Vector3(transform.position.x , CompletePoint.transform.position.y , transform.position.z);
        }
        else
            turning = false;
        // move and turn
        if(moving)
            transform.position = new Vector3(transform.position.x + speed * Time.deltaTime, transform.position.y, transform.position.z);
        if(turning)
        {
            transform.eulerAngles = new Vector3(0, 0, TurnAngle);
            transform.position = new Vector3(transform.position.x , transform.position.y + YSpeed * Time.deltaTime, transform.position.z);
        } 
    }
}
