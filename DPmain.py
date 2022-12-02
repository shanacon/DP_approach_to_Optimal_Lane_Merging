with open("TestData.txt", "r") as data :
    Atimes = list(map(float, data.readline().split()))
    Btimes = list(map(float, data.readline().split()))
WEQUAL = 1.0
WDIFF = 3.0
alpha = len(Atimes) - 1
beta = len(Btimes) - 1
LA = [[0] * (beta + 1) for i in range(alpha + 1)]
LB = [[0] * (beta + 1) for i in range(alpha + 1)]
RA = [[-1] * (beta + 1) for i in range(alpha + 1)] ## record order :0 mean from a, 1 mean from b
RB = [[-1] * (beta + 1) for i in range(alpha + 1)] ## record order :0 mean from a, 1 mean from b
## initialze
LA[1][0] = Atimes[1]
LB[0][1] = Btimes[1]
LA[0][1] = float('inf')
LB[1][0] = float('inf')
RA[1][0] = 0
RB[0][1] = 1
for i in range(2, alpha + 1):
    LA[i][0] = max(Atimes[i], LA[i - 1][0] + WEQUAL)
    LB[i][0] = float('inf')
    RA[i][0] = 0
for i in range(2, beta + 1):
    LB[0][i] = max(Btimes[i], LB[0][i - 1] + WEQUAL)
    LA[0][i] = float('inf')
    RB[0][i] = 1
## DP part
for i in range(1, alpha + 1) :
    for j in range(1, beta + 1):
        Amax = max(Atimes[i], LA[i-1][j] + WEQUAL)
        Bmax = max(Atimes[i], LB[i-1][j] + WDIFF)
        LA[i][j] = min(Amax, Bmax)
        if Amax <= Bmax :
            RA[i][j] = 0
        else :
            RA[i][j] = 1
        Amax = max(Btimes[j], LA[i][j-1] + WDIFF)
        Bmax = max(Btimes[j], LB[i][j-1] + WEQUAL)
        LB[i][j] = min(Amax, Bmax)
        if Amax <= Bmax :
            RB[i][j] = 0
        else :
            RB[i][j] = 1
# for i in range(0, alpha + 1) :
#         print(LA[i])
# print()
# for i in range(0, alpha + 1) :
#         print(LB[i])
# print()
# for i in range(0, alpha + 1) :
#         print(RA[i])
# print()
# for i in range(0, alpha + 1) :
#         print(RB[i])
# print()
AorB = -1
IndexI = alpha
IndexJ = beta
TraceRecode = ""
TimeList = []
if LA[alpha][beta] <= LB[alpha][beta] :
    AorB = 0
else :
    AorB = 1
while IndexI != 0 or IndexJ != 0 :
    if AorB == 0 : 
        TraceRecode = "A" + TraceRecode
        AorB = RA[IndexI][IndexJ]
        TimeList.append(LA[IndexI][IndexJ])
        IndexI = IndexI - 1
    else :
        TraceRecode = "B" + TraceRecode
        AorB = RB[IndexI][IndexJ]
        TimeList.append(LB[IndexI][IndexJ])
        IndexJ = IndexJ - 1
TimeList.reverse()
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
## output
print(TraceRecode)
print(TimeList)
print("Tlast : " + str(TimeList[-1]))
print("Tdelay : " + str(Tdelay))