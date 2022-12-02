from numpy import random
## CaseNum
CaseLen = int(input("Please input case length: "))
## CaseNum
CaseNum = int(input("Please input case number: "))
## lamba
lamba = input("Please input lambda: ")
##
CarArriveTimes = []
for c in range(CaseNum) :
    TimesCar = random.poisson(lam = float(lamba), size = CaseLen)
    CarArriveTime = [-1]
    for i in range(len(TimesCar)):
         for j in range(TimesCar[i]) :
            CarArriveTime.append(i+1)
    CarArriveTimes.append(CarArriveTime)
###
with open("TestData.txt", "w") as data :
    for c in range(CaseNum) :
        for item in CarArriveTimes[c]:
            data.write(str(item) + " ")
        data.write("\n")