// FIFOconmain.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
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
    ifstream TimeData;
    TimeData.open("TestData.txt");
    string Aline, Bline, Cline;
    getline(TimeData, Aline);
    getline(TimeData, Bline);
    getline(TimeData, Cline);
    TimeData.close();
    vector<double> Atimes, Btimes, Ctimes;
    Atimes = ToDoubleVecter(split(Aline, ' '));
    Btimes = ToDoubleVecter(split(Bline, ' '));
    Ctimes = ToDoubleVecter(split(Cline, ' '));
    const double W1EQUAL = 1.0;
    const double W1DIFF = 3.0;
    const double W2EQUAL = 1.0;
    const double W2DIFF = 3.0;
    const double Tf = 2.0;
    int alpha = Atimes.size() - 1;
    int beta = Btimes.size() - 1;
    int theta = Ctimes.size() - 1;
    //
    int IndexI = 1;
    int IndexJ = 1;
    int LastCar = -1;  // 0 mean from a, 1 mean from b
    double TimeIndex = -3.0;
    string TraceOne = "";
    vector<double> TimeListAB;
    vector<double> TimeListA1;
    vector<double> TimeListB1;
    while (IndexI < alpha + 1 && IndexJ < beta + 1)
    {
        if (Atimes[IndexI] <= Btimes[IndexJ])
        {
            TraceOne = TraceOne + "0";
            if (LastCar == 0)
                TimeIndex = max(Atimes[IndexI], TimeIndex + W1EQUAL);
            else
                TimeIndex = max(Atimes[IndexI], TimeIndex + W1DIFF);
            TimeListA1.push_back(TimeIndex);
            LastCar = 0;
            IndexI = IndexI + 1;
        }
        else
        {
            TraceOne = TraceOne + "1";
            if (LastCar == 1)
                TimeIndex = max(Btimes[IndexJ], TimeIndex + W1EQUAL);
            else
                TimeIndex = max(Btimes[IndexJ], TimeIndex + W1DIFF);
            TimeListB1.push_back(TimeIndex);
            LastCar = 1;
            IndexJ = IndexJ + 1;
        }
        TimeListAB.push_back(TimeIndex + Tf);
    }
    while (IndexI < alpha + 1)
    {
        TraceOne = TraceOne + "0";
        if (LastCar == 0)
            TimeIndex = max(Atimes[IndexI], TimeIndex + W1EQUAL);
        else
            TimeIndex = max(Atimes[IndexI], TimeIndex + W1DIFF);
        TimeListA1.push_back(TimeIndex);
        TimeListAB.push_back(TimeIndex + Tf);
        LastCar = 0;
        IndexI = IndexI + 1;
    }
    while (IndexJ < beta + 1)
    {
        TraceOne = TraceOne + "1";
        if (LastCar == 1)
            TimeIndex = max(Btimes[IndexJ], TimeIndex + W1EQUAL);
        else
            TimeIndex = max(Btimes[IndexJ], TimeIndex + W1DIFF);
        TimeListB1.push_back(TimeIndex);
        TimeListAB.push_back(TimeIndex + Tf);
        LastCar = 1;
        IndexJ = IndexJ + 1;
    }
    IndexI = 0;
    IndexJ = 1;
    TimeIndex = -3;
    LastCar = -1;
    int ApulusB = TraceOne.size();
    string TraceRecode = "";
    vector<double> TimeListA;
    vector<double> TimeListB;
    vector<double> TimeListC;
    for (double d : TimeListAB)
        cout << d << " ";
    cout << endl;
    for (double d : Ctimes)
        cout << d << " ";
    cout << endl;
    while (IndexI < ApulusB && IndexJ < theta + 1)
    {
        if (TimeListAB[IndexI] <= Ctimes[IndexJ])
        {
            TraceRecode = TraceRecode + TraceOne[IndexI];
            if (LastCar == 0)
                TimeIndex = max(TimeListAB[IndexI], TimeIndex + W2EQUAL);
            else
                TimeIndex = max(TimeListAB[IndexI], TimeIndex + W2DIFF);
            if(TraceOne[IndexI] == '0')
                TimeListA.push_back(TimeIndex);
            else
                TimeListB.push_back(TimeIndex);
            LastCar = 0;
            IndexI++;
        }
        else
        {
            cout << "2 " << IndexI << " " << IndexJ << endl;
            TraceRecode = TraceRecode + "2";
            if (LastCar == 1)
                TimeIndex = max(Ctimes[IndexJ], TimeIndex + W1EQUAL);
            else
                TimeIndex = max(Ctimes[IndexJ], TimeIndex + W1DIFF);
            TimeListC.push_back(TimeIndex);
            LastCar = 1;
            IndexJ++;
        }
        TimeListAB.push_back(TimeIndex + Tf);
    }
    while (IndexI < ApulusB)
    {
        TraceRecode = TraceRecode + TraceOne[IndexI];
        if (LastCar == 0)
            TimeIndex = max(TimeListAB[IndexI], TimeIndex + W2EQUAL);
        else
            TimeIndex = max(TimeListAB[IndexI], TimeIndex + W2DIFF);
        if (TraceOne[IndexI] == '0')
            TimeListA.push_back(TimeIndex);
        else
            TimeListB.push_back(TimeIndex);
        LastCar = 0;
        IndexI++;
    }
    while (IndexJ < theta + 1)
    {
        TraceRecode = TraceRecode + "2";
        if (LastCar == 1)
            TimeIndex = max(Ctimes[IndexI], TimeIndex + W1EQUAL);
        else
            TimeIndex = max(Ctimes[IndexI], TimeIndex + W1DIFF);
        TimeListC.push_back(TimeIndex);
        LastCar = 1;
        IndexJ++;
    }
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
    vector<double> OutputC;
    Lastitem = -2.0;
    for (double item : Ctimes)
    {
        while (item <= Lastitem)
            item = item + 1;
        Lastitem = item;
        OutputC.push_back(item);
    }
    double OutMin = max(TimeListA.back(), TimeListB.back());
    OutMin = max(OutMin, TimeListC.back());
    //
    ofstream ResultData;
    ResultData.open("FIFOconResult.txt");
    for (double item : OutputA)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : OutputB)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : OutputC)
        ResultData << item << " ";
    ResultData << endl;
    ResultData << TraceOne << endl;
    ResultData << TraceRecode << endl;
    for (double item : TimeListA1)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : TimeListB1)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : TimeListA)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : TimeListB)
        ResultData << item << " ";
    ResultData << endl;
    for (double item : TimeListC)
        ResultData << item << " ";
    ResultData << endl;
    ResultData << OutMin << endl;
    ResultData.close();
}
