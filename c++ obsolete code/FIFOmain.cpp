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
    const double WDIFF = 3.0;
    int alpha = Atimes.size() - 1;
    int beta = Btimes.size() - 1;
    int IndexI = 1;
    int IndexJ = 1;
    int LastCar = -1;  // 0 mean from a, 1 mean from b
    double TimeIndex = -3.0;
    string TraceRecode = "";
    vector<double> TimeListA;
    vector<double> TimeListB;
    // FIFO
    while (IndexI < alpha + 1 && IndexJ < beta + 1)
    {
        if (Atimes[IndexI] <= Btimes[IndexJ])
        {
            TraceRecode = TraceRecode + "0";
            if (LastCar == 0)
                TimeIndex = max(Atimes[IndexI], TimeIndex + WEQUAL);
            else if (LastCar == 1)
                TimeIndex = max(Atimes[IndexI], TimeIndex + WDIFF);
            TimeListA.push_back(TimeIndex);
            LastCar = 0;
            IndexI = IndexI + 1;
        }
        else
        {
            TraceRecode = TraceRecode + "1";
            if (LastCar == 1)
                TimeIndex = max(Btimes[IndexJ], TimeIndex + WEQUAL);
            else
                TimeIndex = max(Btimes[IndexJ], TimeIndex + WDIFF);
            TimeListB.push_back(TimeIndex);
            LastCar = 1;
            IndexJ = IndexJ + 1;
        }
    }
    while (IndexI < alpha + 1)
    {
        TraceRecode = TraceRecode + "0";
        if (LastCar == 0)
            TimeIndex = max(Atimes[IndexI], TimeIndex + WEQUAL);
        else
            TimeIndex = max(Atimes[IndexI], TimeIndex + WDIFF);
        TimeListA.push_back(TimeIndex);
        LastCar = 0;
        IndexI = IndexI + 1;
    }
    while (IndexJ < beta + 1)
    {
        TraceRecode = TraceRecode + "1";
        if (LastCar == 1)
            TimeIndex = max(Btimes[IndexJ], TimeIndex + WEQUAL);
        else
            TimeIndex = max(Btimes[IndexJ], TimeIndex + WDIFF);
        TimeListB.push_back(TimeIndex);
        LastCar = 1;
        IndexJ = IndexJ + 1;
    }
    // calculate Tdelay
    IndexI = 1;
    IndexJ = 1;
    double Tdelay = 0.0;
    for (int c = 0;c < TraceRecode.size();c++)
    {
        if (TraceRecode[c] == '0')
        {
            Tdelay = Tdelay + (TimeListA[IndexI - 1] - Atimes[IndexI]);
            IndexI = IndexI + 1;
        }
        else if (TraceRecode[c] == '1')
        {
            Tdelay = Tdelay + (TimeListB[IndexJ - 1] - Btimes[IndexJ]);
            IndexJ = IndexJ + 1;
        }
    }
    Tdelay = float(Tdelay / TraceRecode.size());
    // output
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
    ResultData.open("FIFOResult.txt");
    for (double item : OutputA)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : OutputB)
        ResultData << item << " ";
    ResultData << endl;
    ResultData << TraceRecode << endl;
    for (double item : TimeListA)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : TimeListB)
        ResultData << item << " ";
    ResultData << endl;
    ResultData << max(TimeListA.back(), TimeListB.back()) << endl;
    ResultData << Tdelay << endl;
    ResultData.close();
}