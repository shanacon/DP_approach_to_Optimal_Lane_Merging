// DP_approach_to_Optimal_Lane_Merging.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
const vector<string> split(const string& str, const char& delimiter)
{
    vector<string> result;
    stringstream ss(str);
    string tok;

    while (getline(ss, tok, delimiter)) {
        result.push_back(tok);
    }
    return result;
}
const vector<double> ToDoubleVecter(const vector<string> StrVector)
{
    vector<double> result;
    for (string str : StrVector)
        result.push_back(stod(str));
    return result;
}
int main()
{
    // read data
    ifstream TimeData;
    TimeData.open("TestData.txt");
    string Aline, Bline;
    getline(TimeData, Aline);
    getline(TimeData, Bline);
    TimeData.close();
    vector<double> Atimes, Btimes;
    Atimes = ToDoubleVecter(split(Aline, ' '));
    Btimes = ToDoubleVecter(split(Bline, ' '));
    const double WEQUAL = 1.0;
    const double WDIFF  = 3.0;
    int alpha = Atimes.size() - 1;
    int beta = Btimes.size() - 1;
    // initialize
    vector<vector<double> > LA(alpha + 1,vector<double>(beta + 1));
    vector<vector<double> > LB(alpha + 1, vector<double>(beta + 1));
    vector<vector<double> > RA(alpha + 1, vector<double>(beta + 1, -1));
    vector<vector<double> > RB(alpha + 1, vector<double>(beta + 1, -1));
    LA[1][0] = Atimes[1];
    LB[0][1] = Btimes[1];
    LA[0][1] = float('inf');
    LB[1][0] = float('inf');
    RA[1][0] = 0;
    RB[0][1] = 1;
    for (int i = 2;i < alpha + 1;i++)
    {
        LA[i][0] = max(Atimes[i], LA[i - 1][0] + WEQUAL);
        LB[i][0] = float('inf');
        RA[i][0] = 0;
    }
    for (int i = 2;i < beta + 1;i++)
    {
        LB[0][i] = max(Btimes[i], LB[0][i - 1] + WEQUAL);
        LA[0][i] = float('inf');
        RB[0][i] = 1;
    }
    // DP part
    for (int i = 1;i < alpha + 1;i++)
    {
        for (int j = 1;j < beta + 1;j++)
        {
            double Amax = max(Atimes[i], LA[i - 1][j] + WEQUAL);
            double Bmax = max(Atimes[i], LB[i - 1][j] + WDIFF);
            LA[i][j] = min(Amax, Bmax);
            if (Amax <= Bmax)
                RA[i][j] = 0;
            else
                RA[i][j] = 1;
            Amax = max(Btimes[j], LA[i][j - 1] + WDIFF);
            Bmax = max(Btimes[j], LB[i][j - 1] + WEQUAL);
            LB[i][j] = min(Amax, Bmax);
            if (Amax <= Bmax)
                RB[i][j] = 0;
            else
                RB[i][j] = 1;
        }
    }
    int AorB = -1;
    int IndexI = alpha;
    int IndexJ = beta;
    string TraceRecode = "";
    vector<double> TimeList;
    if (LA[alpha][beta] <= LB[alpha][beta])
        AorB = 0;
    else
        AorB = 1;
    while (IndexI != 0 or IndexJ != 0)
    {
        if (AorB == 0)
        {
            TraceRecode = "A" + TraceRecode;
            AorB = RA[IndexI][IndexJ];
            TimeList.push_back(LA[IndexI][IndexJ]);
            IndexI = IndexI - 1;
        }
        else if(AorB == 1)
        {
            TraceRecode = "B" + TraceRecode;
            AorB = RB[IndexI][IndexJ];
            TimeList.push_back(LB[IndexI][IndexJ]);
            IndexJ = IndexJ - 1;
        }
    }
    reverse(TimeList.begin(), TimeList.end());
    // calculate Tdelay
    IndexI = 1;
    IndexJ = 1;
    double Tdelay = 0.0;
    for (int c = 0;c < TraceRecode.size();c++)
    {
        if (TraceRecode[c] == 'A')
        {
            Tdelay = Tdelay + (TimeList[c] - Atimes[IndexI]);
            IndexI = IndexI + 1;
        }
        else if (TraceRecode[c] == 'B')
        {
            Tdelay = Tdelay + (TimeList[c] - Btimes[IndexJ]);
            IndexJ = IndexJ + 1;
        }
    }
    Tdelay = float(Tdelay / TraceRecode.size());
    // output
    /*cout << TraceRecode << endl;
    for(double d : TimeList)
        cout << d << " ";
    cout << endl;
    cout << "Tlast : " << TimeList.back() << endl;
    cout << "Tdelay : " << Tdelay << endl;*/
    vector<double> OutputA;
    double Lastitem = -2.0;
    for (double item : Atimes)
    {
        while (item <= Lastitem)
            item = item + 1;
        Lastitem = item;
        OutputA.push_back(item);
    }
    vector<double> OutputB;
    Lastitem = -2.0;
    for (double item : Btimes)
    {
        while (item <= Lastitem)
            item = item + 1;
        Lastitem = item;
        OutputB.push_back(item);
    }
    // write result file
    ofstream ResultData;
    ResultData.open("DPResult.txt");
    for (double item : OutputA)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : OutputB)
        ResultData << item << " ";
    ResultData << endl;
    ResultData << TraceRecode << endl;
    for (double item : TimeList)
        ResultData << item << " ";
    ResultData << endl;
    ResultData << TimeList.back() << endl;
    ResultData << Tdelay << endl;
    ResultData.close();
}