with open("TestData.txt", "r") as data :
    Atimes = list(map(float, data.readline().split()))
    Btimes = list(map(float, data.readline().split()))
WEQUAL = 1.0
WDIFF = 3.0
alpha = len(Atimes) - 1
beta = len(Btimes) - 1
IndexI = 1
IndexJ = 1
LastCar = -1        ## 0 mean from a, 1 mean from b
TimeIndex = -3.0
##
TraceRecode = ""
TimeList = []
while IndexI < alpha + 1 and IndexJ < beta + 1 :
    if Atimes[IndexI] <= Btimes[IndexJ] :
        TraceRecode = TraceRecode + "A"
        if LastCar == 0 :
            TimeIndex = max(Atimes[IndexI], TimeIndex + WEQUAL)
        else :
            TimeIndex = max(Atimes[IndexI], TimeIndex + WDIFF)
        TimeList.append(TimeIndex)
        LastCar = 0
        IndexI = IndexI + 1    
    else :
        TraceRecode = TraceRecode + "B"
        if LastCar == 1 :
            TimeIndex = max(Btimes[IndexI], TimeIndex + WEQUAL)
        else :
            TimeIndex = max(Btimes[IndexI], TimeIndex + WDIFF)
        TimeList.append(TimeIndex)
        LastCar = 1
        IndexJ = IndexJ + 1
while IndexI < alpha + 1 :
    TraceRecode = TraceRecode + "A"
    if LastCar == 0 :
            TimeIndex = max(Atimes[IndexI], TimeIndex + WEQUAL)
    else :
        TimeIndex = max(Atimes[IndexI], TimeIndex + WDIFF)
    TimeList.append(TimeIndex)
    LastCar = 0
    IndexI = IndexI + 1 
while IndexJ < beta + 1 :
    TraceRecode = TraceRecode + "B"
    if LastCar == 1 :
        TimeIndex = max(Btimes[IndexI], TimeIndex + WEQUAL)
    else :
        TimeIndex = max(Btimes[IndexI], TimeIndex + WDIFF)
    TimeList.append(TimeIndex)
    LastCar = 1
    IndexJ = IndexJ + 1
## calculate Tdelay
IndexI = 1
IndexJ = 1
Tdelay = 0.0
for c in range(len(TraceRecode)) :
    if TraceRecode[c] == 'A' :
        Tdelay = Tdelay + (TimeList[c] - Atimes[IndexI])
        IndexI = IndexI + 1
    elif TraceRecode[c] == 'B' :
        Tdelay = Tdelay + (TimeList[c] - Btimes[IndexJ])
        IndexJ = IndexJ + 1
Tdelay = float(Tdelay / len(TraceRecode))
print(TraceRecode)
print(TimeList)
print("Tlast : " + str(TimeList[-1]))
print("Tdelay : " + str(Tdelay))
