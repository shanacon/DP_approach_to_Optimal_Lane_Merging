# DP_approach_to_Optimal_Lane_Merging
###### This project is an implement of this paper: [A Dynamic Programming Approach to Optimal Lane Merging of Connected and Autonomous Vehicles](https://ieeexplore.ieee.org/document/9304813)
## Introduction
This paper propose a dynamic programming algorithm to solve lane mering problem.\
Their are mainly two cases had been discussed is this paper.\
Which are **two-lane merging problem** and **consecutive lane merging problem** respectively. The sample scenario is shown below.
### two-lane merging
![two-lane](https://user-images.githubusercontent.com/79785416/209139622-d9dcf874-75be-452d-9122-628488f4dbc5.png)
### consecutive lane merging
![consecutive lane](https://user-images.githubusercontent.com/79785416/209139631-447f4a36-dfbe-41c0-8754-9ee6e62df062.png)
This project implement the algorithm proposed by this paper by c++, and demo the result compare with First-In-First-Out algorithm by unity.
## How to use Demo
![Demo](https://user-images.githubusercontent.com/79785416/209155758-cc66b242-c787-4da7-a8fb-162dd284170f.png)
### W=
When two vehicles consecutively pass the merging point and they are from same incoming lane.\
W= is the minimun time to wait between times of these two vehicles pass the merging point.
### W+
When two vehicles consecutively pass the merging point and they are from diffrent incoming lanes.\
W+ is the minimun time to wait between times of these two vehicles pass the merging point.
### Tf
Only available in **consecutive lane merging**. Which mean the transfer time between first junction and second junction.\
Suppose Tf is same for all cars.
### lambda
Parameter lambda of poisson distribution which use in randomly create car's arrival time.\
The bigger the value is mean there will have higher probability to have cars arrive junction in a unit time.
### CaseLen
Total cars number in each lane.
### TwoLane and ConsecutiveLane
Check one of them to choose which scenario to demo.
### Smaller collider box for cars
Check this if you want a smaller collider box for cars.\
This function is mainly needed in **consecutive lane merging** because the transfer lane is too short to contain all car.
### Run
Run the algorithm and the result will show in the table at right side.
LastTime mean time of the last car to pass the last junction
Tdelay mean the mean delay of each car to pass the junction.\
It will be always -1 when doing **consecutive lane merging**. Because this paper doesn't mention how to calculate Tdelay in **consecutive lane merging**.
### Start
Click it if you want to see the result in detail and in visualization.\
There have no function to back to initial scene yet. So you need to re-open the program if you want to quiz visualization scene and back to initial scene.
## Code file summary  
### LaneMergeDll
This is the dll project which implement the algorithm and return the result to unity.
> #### MainLibrary.h
All function are declare in this code.
> #### dllmain.cpp
All function are implement in this code.
### LaneMergingDEMO
This project use the dll generate by **LaneMergeDll** and demo the result of algorithm.
> #### CarMovement.cs
This code control the car's movement in **two-lane merging problem**.
> #### CarConMovement.cs
This code control the car's movement in **consecutive lane merging problem**./Since there are parameter **Tf** which will affect car's speed in trasfer lane, so we need another script to control car in **consecutive lane merging problem**.
> #### CreateCar.cs
This code control the creation of car to stimulate the car's arrival.
> #### Assets/Script/DllLibrary.cs
This code import the dll function from **LaneMergeDll**.
> #### LaneControl.cs
This code send message to each car which tell them when to cross the junction.
> #### UIControl.cs
This code control the UI of Demo. Also send global time to each lane manager.
## DEMO program
Download and try to use it at [here](https://drive.google.com/file/d/1qSWYHTa4ab9nxCnUXdOyeVTGqBvtpLcu/view?usp=share_link)
