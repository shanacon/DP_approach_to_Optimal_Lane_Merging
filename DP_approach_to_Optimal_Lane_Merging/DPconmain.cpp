// DPconmain.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
struct MinReturn
{
    int index;
    double MinNum;
};
const MinReturn minimum(double a, double b, double c, double d)
{
    MinReturn ret;
    ret.MinNum = min(min(a, b), min(c, d));
    if (ret.MinNum == a)
        ret.index = 0;
    else if (ret.MinNum == b)
        ret.index = 1;
    else if (ret.MinNum == c)
        ret.index = 2;
    else if (ret.MinNum == d)
        ret.index = 3;
    return ret;
}
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
    // initialize
    vector<vector<vector<double> > > L1A(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1)));
    vector<vector<vector<double> > > L1B(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1)));
    vector<vector<vector<double> > > L1Ca(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1)));
    vector<vector<vector<double> > > L1Cb(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1)));
    vector<vector<vector<double> > > L2A(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1)));
    vector<vector<vector<double> > > L2B(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1)));
    vector<vector<vector<double> > > L2Ca(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1)));
    vector<vector<vector<double> > > L2Cb(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1)));
    /// use in backtrace: 0 from A, 1 from B, 2 from Ca, 3 from Cb
    //vector<vector<vector<double> > > R1A(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
    //vector<vector<vector<double> > > R1B(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
    //vector<vector<vector<double> > > R1Ca(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
    //vector<vector<vector<double> > > R1Cb(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
    vector<vector<vector<double> > > R2A(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
    vector<vector<vector<double> > > R2B(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
    vector<vector<vector<double> > > R2Ca(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
    vector<vector<vector<double> > > R2Cb(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
    L1A[0][0][0] = 0;
    L1B[0][0][0] = 0;
    L1Ca[0][0][0] = 0;
    L1Cb[0][0][0] = 0;
    L2A[0][0][0] = 0;
    L2B[0][0][0] = 0;
    L2Ca[0][0][0] = 0;
    L2Cb[0][0][0] = 0;
    for (int i = 1;i < alpha + 1;i++)
    {
        L1A[i][0][0] = max(Atimes[i], L1A[i-1][0][0] + W1EQUAL);
        L1B[i][0][0] = float('inf');
        L1Ca[i][0][0] = max(Atimes[i], L1Ca[i - 1][0][0] + W1EQUAL); //?
        L1Cb[i][0][0] = float('inf');
    }
    for (int i = 1;i < beta + 1;i++)
    {
        L1A[0][i][0] = float('inf');
        L1B[0][i][0] = max(Btimes[i], L1B[0][i - 1][0] + W1EQUAL);
        L1Ca[0][i][0] = float('inf');
        L1Cb[0][i][0] = max(Btimes[i], L1Cb[0][i - 1][0] + W1EQUAL); //?
    }
    for (int i = 1;i < theta + 1;i++)
    {
        L1A[0][0][i] = 0;
        L1B[0][0][i] = 0;
        L1Ca[0][0][i] = 0; //??
        L1Cb[0][0][i] = 0; //??
    }
    for (int i = 1;i < alpha + 1;i++)
    {
        for (int j = 1;j < beta + 1;j++)
        {
            L1A[i][j][0] = min(max(Atimes[i], L1A[i - 1][j][0] + W1EQUAL), max(Atimes[i], L1B[i - 1][j][0] + W1DIFF));
            L1B[i][j][0] = min(max(Btimes[j], L1B[i][j - 1][0] + W1EQUAL), max(Btimes[j], L1A[i][j - 1][0] + W1DIFF));
            L1Ca[i][j][0] = min(max(Atimes[i], L1Ca[i - 1][j][0] + W1EQUAL), max(Atimes[i], L1Cb[i - 1][j][0] + W1DIFF));
            L1Cb[i][j][0] = min(max(Btimes[j], L1Cb[i][j - 1][0] + W1EQUAL), max(Btimes[j], L1Ca[i][j - 1][0] + W1DIFF));
        }
    }
    for (int i = 1;i < alpha + 1;i++)
    {
        for (int j = 1;j < theta + 1;j++)
        {
            L1A[i][0][j] = max(Atimes[i], L1A[i - 1][0][j] + W1EQUAL);
            L1B[i][0][j] = float('inf');
            L1Ca[i][0][j] = max(Atimes[i], L1A[i - 1][0][j] + W1EQUAL);
            L1Cb[i][0][j] = float('inf'); //?
        }
    }
    for (int i = 1;i < beta + 1;i++)
    {
        for (int j = 1;j < theta + 1;j++)
        {
            L1A[0][i][j] = float('inf');
            L1B[0][i][j] = max(Btimes[i], L1B[0][i - 1][j] + W1EQUAL);
            L1Ca[0][i][j] = float('inf'); //?
            L1Cb[0][i][j] = max(Btimes[i], L1B[0][i - 1][j] + W1EQUAL);
        }
    }
    for (int i = 1;i < alpha + 1;i++)
    {
        L2A[i][0][0] = max(Atimes[i] + Tf, L2A[i - 1][0][0] + W2EQUAL);
        R2A[i][0][0] = 0;
        L2B[i][0][0] = float('inf');
        L2Ca[i][0][0] = float('inf');
        L2Cb[i][0][0] = float('inf');
    } 
    for (int i = 1;i < beta + 1;i++)
    {
        L2A[0][i][0] = float('inf');
        L2B[0][i][0] = max(Btimes[i] + Tf, L2B[0][i - 1][0] + W2EQUAL);
        R2B[0][i][0] = 1;
        L2Ca[0][i][0] = float('inf');
        L2Cb[0][i][0] = float('inf');
    }
    for (int i = 1;i < theta + 1;i++)
    {
        L2A[0][0][i] = float('inf');
        L2B[0][0][i] = float('inf');
        L2Ca[0][0][i] = max(Ctimes[i], L2Ca[0][0][i - 1] + W2EQUAL); //?
        L2Cb[0][0][i] = max(Ctimes[i], L2Cb[0][0][i - 1] + W2EQUAL); //?
        R2Ca[0][0][i] = 2;
        R2Cb[0][0][i] = 3;
    }
    MinReturn ret;
    for (int i = 1;i < alpha + 1;i++)
    {
        for (int j = 1;j < beta + 1;j++)
        {
            ret = minimum( max(max(Atimes[i], L1A[i - 1][j][0] + W1EQUAL) + Tf, L2A[i - 1][j][0] + W2EQUAL),
                           max(max(Atimes[i], L1B[i - 1][j][0] + W1DIFF) + Tf, L2B[i - 1][j][0] + W2EQUAL),
                           max(max(Atimes[i], L1Ca[i - 1][j][0] + W1EQUAL) + Tf, L2Ca[i - 1][j][0] + W1DIFF),
                           max(max(Atimes[i], L1Cb[i - 1][j][0] + W1DIFF) + Tf, L2Cb[i - 1][j][0] + W1DIFF));
            L2A[i][j][0] = ret.MinNum;
            R2A[i][j][0] = ret.index;
            //
            ret = minimum( max(max(Btimes[j], L1A[i][j - 1][0] + W1DIFF) + Tf, L2A[i][j - 1][0] + W2EQUAL),
                           max(max(Btimes[j], L1B[i][j - 1][0] + W1EQUAL) + Tf, L2B[i][j - 1][0] + W2EQUAL),
                           max(max(Btimes[j], L1Ca[i][j - 1][0] + W1DIFF) + Tf, L2Ca[i][j - 1][0] + W1DIFF),
                           max(max(Btimes[j], L1Cb[i][j - 1][0] + W1EQUAL) + Tf, L2Cb[i][j - 1][0] + W1DIFF));
            L2B[i][j][0] = ret.MinNum;
            R2B[i][j][0] = ret.index;
            L2Ca[i][j][0] = float('inf');
            L2Cb[i][j][0] = float('inf');
        }
    }
    for (int i = 1;i < alpha + 1;i++)
    {
        for (int j = 1;j < theta + 1;j++)
        {
            ret = minimum( max(max(Atimes[i], L1A[i - 1][0][j] + W1EQUAL) + Tf, L2A[i - 1][0][j] + W2EQUAL),
                           max(max(Atimes[i], L1B[i - 1][0][j] + W1DIFF) + Tf, L2B[i - 1][0][j] + W2EQUAL),
                           max(max(Atimes[i], L1Ca[i - 1][0][j] + W1EQUAL) + Tf, L2Ca[i - 1][0][j] + W1DIFF),
                           max(max(Atimes[i], L1Cb[i - 1][0][j] + W1DIFF) + Tf, L2Cb[i - 1][0][j] + W1DIFF));
            L2A[i][0][j] = ret.MinNum;
            R2A[i][0][j] = ret.index;
            L2B[i][0][j] = float('inf');
            double max1 = max(Ctimes[j], L2A[i][0][j - 1] + W2DIFF);
            double max2 = max(Ctimes[j], L2Ca[i][0][j - 1] + W2EQUAL);
            L2Ca[i][0][j] = min(max1, max2);
            if (L2Ca[i][0][j] == max1)
                R2Ca[i][0][j] = 0;
            if (L2Ca[i][0][j] == max2)
                R2Ca[i][0][j] = 2;
            L2Cb[i][0][j] = float('inf');
        }
    }
    for (int i = 1;i < beta + 1;i++)
    {
        for (int j = 1;j < theta + 1;j++)
        {
            L2A[0][i][j] = float('inf');
            ret = minimum( max(max(Btimes[i], L1A[0][i - 1][j] + W1DIFF) + Tf, L2A[0][i - 1][j] + W2EQUAL),
                           max(max(Btimes[i], L1B[0][i - 1][j] + W1EQUAL) + Tf, L2B[0][i - 1][j] + W2EQUAL),
                           max(max(Btimes[i], L1Ca[0][i - 1][j] + W1DIFF) + Tf, L2Ca[0][i - 1][j] + W1DIFF),
                           max(max(Btimes[i], L1Cb[0][i - 1][j] + W1EQUAL) + Tf, L2Cb[0][i - 1][j] + W1DIFF));
            L2B[0][i][j] = ret.MinNum;
            R2B[0][i][j] = ret.index;
            L2Ca[0][i][j] = float('inf');
            double max1 = max(Ctimes[j], L2B[0][i][j - 1] + W2DIFF);
            double max2 = max(Ctimes[j], L2Ca[0][i][j - 1] + W2EQUAL);
            L2Cb[0][i][j] = min(max1, max2);
            if (L2Cb[0][i][j] == max1)
                R2Cb[0][i][j] = 1;
            if (L2Cb[0][i][j] == max2)
                R2Cb[0][i][j] = 3;
        }
    }
    // dp part
    for (int i = 1;i < alpha + 1;i++)
    {
        for (int j = 1;j < beta + 1;j++)
        {
            for (int k = 1;k < theta + 1;k++)
            {
                L1A[i][j][k] = min(max(Atimes[i], L1A[i - 1][j][k] + W1EQUAL), max(Atimes[i], L1B[i - 1][j][k] + W1DIFF));
                L1B[i][j][k] = min(max(Btimes[j], L1B[i][j - 1][k] + W1EQUAL), max(Btimes[j], L1A[i][j - 1][k] + W1DIFF));
                L1Ca[i][j][k] = min(max(Atimes[i], L1Ca[i - 1][j][k] + W1EQUAL), max(Atimes[i], L1Cb[i - 1][j][k] + W1DIFF));
                L1Cb[i][j][k] = min(max(Btimes[j], L1Cb[i][j - 1][k] + W1EQUAL), max(Btimes[j], L1Ca[i][j - 1][k] + W1DIFF));
                //cout << L1A[i][j][k] << " " << L1B[i][j][k] << " " << L1Ca[i][j][k] << " " << L1Cb[i][j][k] << endl;
                ret = minimum( max(max(Atimes[i], L1A[i - 1][j][k] + W1EQUAL) + Tf, L2A[i - 1][j][k] + W2EQUAL),
                               max(max(Atimes[i], L1B[i - 1][j][k] + W1DIFF) + Tf, L2B[i - 1][j][k] + W2EQUAL),
                               max(max(Atimes[i], L1Ca[i - 1][j][k] + W1EQUAL) + Tf, L2Ca[i - 1][j][k] + W1DIFF),
                               max(max(Atimes[i], L1Cb[i - 1][j][k] + W1DIFF) + Tf, L2Cb[i - 1][j][k] + W1DIFF));
                L2A[i][j][k] = ret.MinNum;
                R2A[i][j][k] = ret.index;
                //cout << "L2A " << i << " " << j << " " << k << " " << L2A[i][j][k] << " " << ret.index << endl;
                //cout << L1B[i][j][k] + Tf << " " << L2B[i - 1][j][k] + W2EQUAL << endl;
                ret = minimum( max(max(Btimes[j], L1A[i][j - 1][k] + W1DIFF) + Tf, L2A[i][j - 1][k] + W2EQUAL),
                               max(max(Btimes[j], L1B[i][j - 1][k] + W1EQUAL) + Tf, L2B[i][j - 1][k] + W2EQUAL),
                               max(max(Btimes[j], L1Ca[i][j - 1][k] + W1DIFF) + Tf, L2Ca[i][j - 1][k] + W1DIFF),
                               max(max(Btimes[j], L1Cb[i][j - 1][k] + W1EQUAL) + Tf, L2Cb[i][j - 1][k] + W1DIFF));
                L2B[i][j][k] = ret.MinNum;
                R2B[i][j][k] = ret.index;
                //cout << "L2B " << i << " " << j << " " << k << " " << L2B[i][j][k] << endl;
                ////
                double max1 = max(Ctimes[k], L2A[i][j][k - 1] + W2DIFF);
                double max2 = max(Ctimes[k], L2Ca[i][j][k - 1] + W2EQUAL);
                L2Ca[i][j][k] = min(max1, max2);
                if (L2Ca[i][j][k] == max1)
                    R2Ca[i][j][k] = 0;
                if (L2Ca[i][j][k] == max2)
                    R2Ca[i][j][k] = 2;
                //cout << "L2Ca " << i << " " << j << " " << k << " " << L2Ca[i][j][k] << endl;
                ///
                max1 = max(Ctimes[k], L2B[i][j][k - 1] + W2DIFF);
                max2 = max(Ctimes[k], L2Ca[i][j][k - 1] + W2EQUAL);
                L2Cb[i][j][k] = min(max1, max2);
                if (L2Cb[i][j][k] == max1)
                    R2Cb[i][j][k] = 1;
                if (L2Cb[i][j][k] == max2)
                    R2Cb[i][j][k] = 3;
               // cout << "L2Cb " << i << " " << j << " " << k << " " << L2Cb[i][j][k] << endl<<endl;
            }
        }
    }
    // trace back
    //cout << L2A[alpha][beta][theta] << " " << L2B[alpha][beta][theta] << " " << L2Ca[alpha][beta][theta] << " " << L2Cb[alpha][beta][theta] << endl;
    ret = minimum(L2A[alpha][beta][theta], L2B[alpha][beta][theta], L2Ca[alpha][beta][theta], L2Cb[alpha][beta][theta]);
    int IndexI = alpha, IndexJ = beta, IndexK = theta, From = ret.index;
    string TraceRecode = "";
    vector<double> TimeListA1;
    vector<double> TimeListB1;
    vector<double> TimeListA;
    vector<double> TimeListB;
    vector<double> TimeListC;
    //
    From = 1;
    while (IndexI > 0 || IndexJ > 0 || IndexK > 0)
    {
        //cout << IndexI << " " << IndexJ << " " << IndexK << endl;
        //cout << From << endl;
        if (From == 0)
        {
            TraceRecode = "0" + TraceRecode;
            From = R2A[IndexI][IndexJ][IndexK];
            TimeListA.push_back(L2A[IndexI][IndexJ][IndexK]);
            IndexI--;
        }
        else if (From == 1)
        {
            TraceRecode = "1" + TraceRecode;
            From = R2B[IndexI][IndexJ][IndexK];
            TimeListB.push_back(L2B[IndexI][IndexJ][IndexK]);
            IndexJ--;
        }
        else if (From == 2)
        {
            TraceRecode = "2" + TraceRecode;
            From = R2Ca[IndexI][IndexJ][IndexK];
            TimeListC.push_back(L2Ca[IndexI][IndexJ][IndexK]);
            IndexK--;
        }
        else if (From == 3)
        {
            TraceRecode = "2" + TraceRecode;
            From = R2Cb[IndexI][IndexJ][IndexK];
            TimeListC.push_back(L2Cb[IndexI][IndexJ][IndexK]);
            IndexK--;
        }
    }
    cout << endl;
    reverse(TimeListA.begin(), TimeListA.end());
    reverse(TimeListB.begin(), TimeListB.end());
    reverse(TimeListC.begin(), TimeListC.end());
    string TraceOne = TraceRecode;
    TraceOne.erase(remove(TraceOne.begin(), TraceOne.end(), '2'), TraceOne.end());
    cout << TraceRecode << endl;
    cout << TraceOne << endl;
    IndexI = 0;
    IndexJ = 0;
    char IndexLast = 'a';
    double IndexTime = 0;
    for (char c : TraceOne)
    {
        if (c == '0')
        {
            int w = W1EQUAL;
            if (IndexLast == '0')
                w = W1EQUAL;
            else if (IndexLast == '1')
                w = W1DIFF;
            IndexLast = c;
            TimeListA1.push_back(max(Atimes[IndexI], IndexTime + w));
            IndexTime = TimeListA1.back();
            IndexI++;
        }
        else if (c == '1')
        {
            int w = W1EQUAL;
            if (IndexLast == '0')
                w = W1DIFF;
            else if (IndexLast == '1')
                w = W1EQUAL;
            IndexLast = c;
            TimeListB1.push_back(max(Atimes[IndexJ], IndexTime + w));
            IndexTime = TimeListB1.back();
            IndexJ++;
        }
    }
    for (double d : TimeListA1)
        cout << d << " ";
    cout << endl;
    for (double d : TimeListB1)
        cout << d << " ";
    cout << endl << endl;
    for (double d : TimeListA)
        cout << d << " ";
    cout << endl;
    for (double d : TimeListB)
        cout << d << " ";
    cout << endl;
    for (double d : TimeListC)
        cout << d << " ";
    cout << endl;
    // output
    vector<double> OutputA;
    vector<double> OutputB;
    vector<double> OutputC;
    double Lastitem = -2.0;
    // adjust times array
    for (double item : Atimes)
    {
        while (item <= Lastitem)
            item = item + 1;
        Lastitem = item;
        OutputA.push_back(item);
    }
    Lastitem = -2.0;
    for (double item : Btimes)
    {
        while (item <= Lastitem)
            item = item + 1;
        Lastitem = item;
        OutputB.push_back(item);
    }
    Lastitem = -2.0;
    for (double item : Ctimes)
    {
        while (item <= Lastitem)
            item = item + 1;
        Lastitem = item;
        OutputC.push_back(item);
    }
    ofstream ResultData;
    ResultData.open("DPconResult.txt");
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
    ResultData << ret.MinNum << endl;
    ResultData.close();
}