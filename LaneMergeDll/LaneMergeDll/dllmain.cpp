#include "pch.h" 
#include "MainLibrary.h"
#include <utility>
#include <limits.h>
#include <vector>
#include <random>
using namespace std;

static double lamba;
static int CaseLen;
static int CaseNum;
static float WEQUAL;
static float WDIFF;
static float Tf;
static vector<float> ATimes;
static vector<float> BTimes;
static vector<float> CTimes;
static vector<float> ARealTimes;
static vector<float> BRealTimes;
static vector<float> CRealTimes;
static vector<float> TimeListA1;
static vector<float> TimeListB1;
static vector<float> TimeListA;
static vector<float> TimeListB;
static vector<float> TimeListC;
static float FinalTime;
static float Tdelay;
static default_random_engine generator;
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
void SetUpRandom(double lamba_, int CaseLen_, int CaseNum_, float Wequal_, float Wdiff_, float Tf_)
{
    lamba = lamba_;
    CaseLen = CaseLen_;
    CaseNum = CaseNum_;
	WEQUAL = Wequal_;
	WDIFF = Wdiff_;
	Tf = Tf_;
    poisson_distribution<int> distribution(lamba);
	ATimes.clear();
	BTimes.clear();
	CTimes.clear();
	ARealTimes.clear();
	BRealTimes.clear();
	CRealTimes.clear();
	vector<int> tmp;
	if (CaseNum >= 1)
	{
		for (int j = 0; j < CaseLen; j++)
		{
			int number = distribution(generator);
			tmp.push_back(number);
		}
		ATimes.push_back(-1);
		for (int j = 0; j < CaseLen; j++)
		{
			for (int k = 0;k < tmp[j];k++)
				ATimes.push_back(j + 1);
		}
		int Lastitem = -2;
		for (int item : ATimes)
		{
			while (item <= Lastitem)
				item = item + 1;
			Lastitem = item;
			ARealTimes.push_back(item);
		}
	}
	tmp.clear();
	if (CaseNum >= 2)
	{
		for (int j = 0; j < CaseLen; j++)
		{
			int number = distribution(generator);
			tmp.push_back(number);
		}
		BTimes.push_back(-1);
		for (int j = 0; j < CaseLen; j++)
		{
			for (int k = 0;k < tmp[j];k++)
				BTimes.push_back(j + 1);
		}
		int Lastitem = -2;
		for (int item : BTimes)
		{
			while (item <= Lastitem)
				item = item + 1;
			Lastitem = item;
			BRealTimes.push_back(item);
		}
	}
	tmp.clear();
	if (CaseNum >= 3)
	{
		for (int j = 0; j < CaseLen; j++)
		{
			int number = distribution(generator);
			tmp.push_back(number);
		}
		CTimes.push_back(-1);
		for (int j = 0; j < CaseLen; j++)
		{
			for (int k = 0;k < tmp[j];k++)
				CTimes.push_back(j + 1);
		}
		int Lastitem = -2;
		for (int item : CTimes)
		{
			while (item <= Lastitem)
				item = item + 1;
			Lastitem = item;
			CRealTimes.push_back(item);
		}
	}
}
int GetTimeList(int LaneNum, int* arr)
{
	if (LaneNum == 1)
	{
		for (int i = 0;i < ATimes.size();i++)
			arr[i] = ATimes[i];
		return ATimes.size();
	}
	if (LaneNum == 2)
	{
		for (int i = 0;i < BTimes.size();i++)
			arr[i] = BTimes[i];
		return BTimes.size();
	}
	if (LaneNum == 3)
	{
		for (int i = 0;i < CTimes.size();i++)
			arr[i] = CTimes[i];
		return CTimes.size();
	}
	return -1;
}
int GetRealTime(int LaneNum, int* RealTime)
{
	if (LaneNum == 1)
	{
		for (int i = 0;i < ARealTimes.size();i++)
			RealTime[i] = ARealTimes[i];
		return ARealTimes.size();
	}
	if (LaneNum == 2)
	{
		for (int i = 0;i < BRealTimes.size();i++)
			RealTime[i] = BRealTimes[i];
		return BRealTimes.size();
	}
	if (LaneNum == 3)
	{
		for (int i = 0;i < CRealTimes.size();i++)
			RealTime[i] = CRealTimes[i];
		return CRealTimes.size();
	}
}
void DoDP()
{
	TimeListA.clear();
	TimeListB.clear();
	TimeListC.clear();
	int alpha = ATimes.size() - 1;
	int beta = BTimes.size() - 1;
	// initialize
	vector<vector<float> > LA(alpha + 1, vector<float>(beta + 1));
	vector<vector<float> > LB(alpha + 1, vector<float>(beta + 1));
	vector<vector<float> > RA(alpha + 1, vector<float>(beta + 1, -1));
	vector<vector<float> > RB(alpha + 1, vector<float>(beta + 1, -1));
	LA[1][0] = ATimes[1];
	LB[0][1] = BTimes[1];
	LA[0][1] = float('inf');
	LB[1][0] = float('inf');
	RA[1][0] = 0;
	RB[0][1] = 1;
	for (int i = 2;i < alpha + 1;i++)
	{
		LA[i][0] = max(ATimes[i], LA[i - 1][0] + WEQUAL);
		LB[i][0] = float('inf');
		RA[i][0] = 0;
	}
	for (int i = 2;i < beta + 1;i++)
	{
		LB[0][i] = max(BTimes[i], LB[0][i - 1] + WEQUAL);
		LA[0][i] = float('inf');
		RB[0][i] = 1;
	}
	// DP part
	for (int i = 1;i < alpha + 1;i++)
	{
		for (int j = 1;j < beta + 1;j++)
		{
			float Amax = max(ATimes[i], LA[i - 1][j] + WEQUAL);
			float Bmax = max(ATimes[i], LB[i - 1][j] + WDIFF);
			LA[i][j] = min(Amax, Bmax);
			if (Amax <= Bmax)
				RA[i][j] = 0;
			else
				RA[i][j] = 1;
			Amax = max(BTimes[j], LA[i][j - 1] + WDIFF);
			Bmax = max(BTimes[j], LB[i][j - 1] + WEQUAL);
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
	if (LA[alpha][beta] <= LB[alpha][beta])
		AorB = 0;
	else
		AorB = 1;
	while (IndexI != 0 or IndexJ != 0)
	{
		if (AorB == 0)
		{
			TraceRecode = "0" + TraceRecode;
			AorB = RA[IndexI][IndexJ];
			TimeListA.push_back(LA[IndexI][IndexJ]);
			IndexI = IndexI - 1;
		}
		else if (AorB == 1)
		{
			TraceRecode = "1" + TraceRecode;
			AorB = RB[IndexI][IndexJ];
			TimeListB.push_back(LB[IndexI][IndexJ]);
			IndexJ = IndexJ - 1;
		}
	}
	reverse(TimeListA.begin(), TimeListA.end());
	reverse(TimeListB.begin(), TimeListB.end());
	// calculate Tdelay
	IndexI = 1;
	IndexJ = 1;
	for (int c = 0;c < TraceRecode.size();c++)
	{
		if (TraceRecode[c] == '0')
		{
			Tdelay = Tdelay + (TimeListA[IndexI - 1] - ATimes[IndexI]);
			IndexI = IndexI + 1;
		}
		else if (TraceRecode[c] == '1')
		{
			Tdelay = Tdelay + (TimeListB[IndexJ - 1] - BTimes[IndexJ]);
			IndexJ = IndexJ + 1;
		}
	}
	Tdelay = float(Tdelay / TraceRecode.size());
	FinalTime = max(TimeListA.back(), TimeListB.back());
}
void DoFIFO()
{
	TimeListA.clear();
	TimeListB.clear();
	TimeListC.clear();
	int alpha = ATimes.size() - 1;
	int beta = BTimes.size() - 1;
	int IndexI = 1;
	int IndexJ = 1;
	int LastCar = -1;  // 0 mean from a, 1 mean from b
	float TimeIndex = -3;
	string TraceRecode = "";
	// FIFO
	while (IndexI < alpha + 1 && IndexJ < beta + 1)
	{
		if (ATimes[IndexI] <= BTimes[IndexJ])
		{
			TraceRecode = TraceRecode + "0";
			if (LastCar == 0)
				TimeIndex = max(ATimes[IndexI], TimeIndex + WEQUAL);
			else
				TimeIndex = max(ATimes[IndexI], TimeIndex + WDIFF);
			TimeListA.push_back(TimeIndex);
			LastCar = 0;
			IndexI = IndexI + 1;
		}
		else
		{
			TraceRecode = TraceRecode + "1";
			if (LastCar == 1)
				TimeIndex = max(BTimes[IndexJ], TimeIndex + WEQUAL);
			else
				TimeIndex = max(BTimes[IndexJ], TimeIndex + WDIFF);
			TimeListB.push_back(TimeIndex);
			LastCar = 1;
			IndexJ = IndexJ + 1;
		}
	}
	while (IndexI < alpha + 1)
	{
		TraceRecode = TraceRecode + "0";
		if (LastCar == 0)
			TimeIndex = max(ATimes[IndexI], TimeIndex + WEQUAL);
		else
			TimeIndex = max(ATimes[IndexI], TimeIndex + WDIFF);
		TimeListA.push_back(TimeIndex);
		LastCar = 0;
		IndexI = IndexI + 1;
	}
	while (IndexJ < beta + 1)
	{
		TraceRecode = TraceRecode + "1";
		if (LastCar == 1)
			TimeIndex = max(BTimes[IndexJ], TimeIndex + WEQUAL);
		else
			TimeIndex = max(BTimes[IndexJ], TimeIndex + WDIFF);
		TimeListB.push_back(TimeIndex);
		LastCar = 1;
		IndexJ = IndexJ + 1;
	}
	// calculate Tdelay
	IndexI = 1;
	IndexJ = 1;
	for (int c = 0;c < TraceRecode.size();c++)
	{
		if (TraceRecode[c] == '0')
		{
			Tdelay = Tdelay + (TimeListA[IndexI - 1] - ATimes[IndexI]);
			IndexI = IndexI + 1;
		}
		else if (TraceRecode[c] == '1')
		{
			Tdelay = Tdelay + (TimeListB[IndexJ - 1] - ATimes[IndexJ]);
			IndexJ = IndexJ + 1;
		}
	}
	Tdelay = float(Tdelay / TraceRecode.size());
	FinalTime = max(TimeListA.back(), TimeListB.back());
}
void DoConDP()
{
	TimeListA.clear();
	TimeListB.clear();
	TimeListC.clear();
	TimeListA1.clear();
	TimeListB1.clear();
	float W1EQUAL = WEQUAL;
	float W1DIFF = WDIFF;
	float W2EQUAL = WEQUAL;
	float W2DIFF = WDIFF;
	int alpha = ATimes.size() - 1;
	int beta = BTimes.size() - 1;
	int theta = CTimes.size() - 1;
	// initialize
	vector<vector<vector<float> > > L1A(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1)));
	vector<vector<vector<float> > > L1B(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1)));
	vector<vector<vector<float> > > L1Ca(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1)));
	vector<vector<vector<float> > > L1Cb(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1)));
	vector<vector<vector<float> > > L2A(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1)));
	vector<vector<vector<float> > > L2B(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1)));
	vector<vector<vector<float> > > L2Ca(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1)));
	vector<vector<vector<float> > > L2Cb(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1)));
	/// use in backtrace: 0 from A, 1 from B, 2 from Ca, 3 from Cb
	//vector<vector<vector<double> > > R1A(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
	//vector<vector<vector<double> > > R1B(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
	//vector<vector<vector<double> > > R1Ca(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
	//vector<vector<vector<double> > > R1Cb(alpha + 1, vector<vector<double> >(beta + 1, vector <double>(theta + 1, -1)));
	vector<vector<vector<float> > > R2A(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1, -1)));
	vector<vector<vector<float> > > R2B(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1, -1)));
	vector<vector<vector<float> > > R2Ca(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1, -1)));
	vector<vector<vector<float> > > R2Cb(alpha + 1, vector<vector<float> >(beta + 1, vector <float>(theta + 1, -1)));
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
		L1A[i][0][0] = max(ATimes[i], L1A[i - 1][0][0] + W1EQUAL);
		L1B[i][0][0] = float('inf');
		L1Ca[i][0][0] = float('inf'); //?
		L1Cb[i][0][0] = float('inf');
	}
	for (int i = 1;i < beta + 1;i++)
	{
		L1A[0][i][0] = float('inf');
		L1B[0][i][0] = max(BTimes[i], L1B[0][i - 1][0] + W1EQUAL);
		L1Ca[0][i][0] = float('inf');
		L1Cb[0][i][0] = float('inf'); //?
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
			L1A[i][j][0] = min(max(ATimes[i], L1A[i - 1][j][0] + W1EQUAL), max(ATimes[i], L1B[i - 1][j][0] + W1DIFF));
			L1B[i][j][0] = min(max(BTimes[j], L1B[i][j - 1][0] + W1EQUAL), max(BTimes[j], L1A[i][j - 1][0] + W1DIFF));
			L1Ca[i][j][0] = min(max(ATimes[i], L1Ca[i - 1][j][0] + W1EQUAL), max(ATimes[i], L1Cb[i - 1][j][0] + W1DIFF));
			L1Cb[i][j][0] = min(max(BTimes[j], L1Cb[i][j - 1][0] + W1EQUAL), max(BTimes[j], L1Ca[i][j - 1][0] + W1DIFF));
		}
	}
	for (int i = 1;i < alpha + 1;i++)
	{
		for (int j = 1;j < theta + 1;j++)
		{
			L1A[i][0][j] = max(ATimes[i], L1A[i - 1][0][j] + W1EQUAL);
			L1B[i][0][j] = float('inf');
			L1Ca[i][0][j] = max(ATimes[i], L1A[i - 1][0][j] + W1EQUAL);
			L1Cb[i][0][j] = float('inf'); //?
		}
	}
	for (int i = 1;i < beta + 1;i++)
	{
		for (int j = 1;j < theta + 1;j++)
		{
			L1A[0][i][j] = float('inf');
			L1B[0][i][j] = max(BTimes[i], L1B[0][i - 1][j] + W1EQUAL);
			L1Ca[0][i][j] = float('inf'); //?
			L1Cb[0][i][j] = max(BTimes[i], L1B[0][i - 1][j] + W1EQUAL);
		}
	}
	for (int i = 1;i < alpha + 1;i++)
	{
		L2A[i][0][0] = max(ATimes[i] + Tf, L2A[i - 1][0][0] + W2EQUAL);
		R2A[i][0][0] = 0;
		L2B[i][0][0] = float('inf');
		L2Ca[i][0][0] = float('inf');
		L2Cb[i][0][0] = float('inf');
	}
	for (int i = 1;i < beta + 1;i++)
	{
		L2A[0][i][0] = float('inf');
		L2B[0][i][0] = max(BTimes[i] + Tf, L2B[0][i - 1][0] + W2EQUAL);
		R2B[0][i][0] = 1;
		L2Ca[0][i][0] = float('inf');
		L2Cb[0][i][0] = float('inf');
	}
	for (int i = 1;i < theta + 1;i++)
	{
		L2A[0][0][i] = float('inf');
		L2B[0][0][i] = float('inf');
		L2Ca[0][0][i] = max(CTimes[i], L2Ca[0][0][i - 1] + W2EQUAL); //?
		L2Cb[0][0][i] = max(CTimes[i], L2Cb[0][0][i - 1] + W2EQUAL); //?
		R2Ca[0][0][i] = 2;
		R2Cb[0][0][i] = 3;
	}
	MinReturn ret;
	for (int i = 1;i < alpha + 1;i++)
	{
		for (int j = 1;j < beta + 1;j++)
		{
			ret = minimum(max(max(ATimes[i], L1A[i - 1][j][0] + W1EQUAL) + Tf, L2A[i - 1][j][0] + W2EQUAL),
				max(max(ATimes[i], L1B[i - 1][j][0] + W1DIFF) + Tf, L2B[i - 1][j][0] + W2EQUAL),
				max(max(ATimes[i], L1Ca[i - 1][j][0] + W1EQUAL) + Tf, L2Ca[i - 1][j][0] + W1DIFF),
				max(max(ATimes[i], L1Cb[i - 1][j][0] + W1DIFF) + Tf, L2Cb[i - 1][j][0] + W1DIFF));
			L2A[i][j][0] = ret.MinNum;
			R2A[i][j][0] = ret.index;
			//
			ret = minimum(max(max(BTimes[j], L1A[i][j - 1][0] + W1DIFF) + Tf, L2A[i][j - 1][0] + W2EQUAL),
				max(max(BTimes[j], L1B[i][j - 1][0] + W1EQUAL) + Tf, L2B[i][j - 1][0] + W2EQUAL),
				max(max(BTimes[j], L1Ca[i][j - 1][0] + W1DIFF) + Tf, L2Ca[i][j - 1][0] + W1DIFF),
				max(max(BTimes[j], L1Cb[i][j - 1][0] + W1EQUAL) + Tf, L2Cb[i][j - 1][0] + W1DIFF));
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
			ret = minimum(max(max(ATimes[i], L1A[i - 1][0][j] + W1EQUAL) + Tf, L2A[i - 1][0][j] + W2EQUAL),
				max(max(ATimes[i], L1B[i - 1][0][j] + W1DIFF) + Tf, L2B[i - 1][0][j] + W2EQUAL),
				max(max(ATimes[i], L1Ca[i - 1][0][j] + W1EQUAL) + Tf, L2Ca[i - 1][0][j] + W1DIFF),
				max(max(ATimes[i], L1Cb[i - 1][0][j] + W1DIFF) + Tf, L2Cb[i - 1][0][j] + W1DIFF));
			L2A[i][0][j] = ret.MinNum;
			R2A[i][0][j] = ret.index;
			L2B[i][0][j] = float('inf');
			double max1 = max(CTimes[j], L2A[i][0][j - 1] + W2DIFF);
			double max2 = max(CTimes[j], L2Ca[i][0][j - 1] + W2EQUAL);
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
			ret = minimum(max(max(BTimes[i], L1A[0][i - 1][j] + W1DIFF) + Tf, L2A[0][i - 1][j] + W2EQUAL),
				max(max(BTimes[i], L1B[0][i - 1][j] + W1EQUAL) + Tf, L2B[0][i - 1][j] + W2EQUAL),
				max(max(BTimes[i], L1Ca[0][i - 1][j] + W1DIFF) + Tf, L2Ca[0][i - 1][j] + W1DIFF),
				max(max(BTimes[i], L1Cb[0][i - 1][j] + W1EQUAL) + Tf, L2Cb[0][i - 1][j] + W1DIFF));
			L2B[0][i][j] = ret.MinNum;
			R2B[0][i][j] = ret.index;
			L2Ca[0][i][j] = float('inf');
			int max1 = max(CTimes[j], L2B[0][i][j - 1] + W2DIFF);
			int max2 = max(CTimes[j], L2Ca[0][i][j - 1] + W2EQUAL);
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
				L1A[i][j][k] = min(max(ATimes[i], L1A[i - 1][j][k] + W1EQUAL), max(ATimes[i], L1B[i - 1][j][k] + W1DIFF));
				L1B[i][j][k] = min(max(BTimes[j], L1B[i][j - 1][k] + W1EQUAL), max(BTimes[j], L1A[i][j - 1][k] + W1DIFF));
				L1Ca[i][j][k] = min(max(ATimes[i], L1Ca[i - 1][j][k] + W1EQUAL), max(ATimes[i], L1Cb[i - 1][j][k] + W1DIFF));
				L1Cb[i][j][k] = min(max(BTimes[j], L1Cb[i][j - 1][k] + W1EQUAL), max(BTimes[j], L1Ca[i][j - 1][k] + W1DIFF));
				//cout << L1A[i][j][k] << " " << L1B[i][j][k] << " " << L1Ca[i][j][k] << " " << L1Cb[i][j][k] << endl;
				ret = minimum(max(max(ATimes[i], L1A[i - 1][j][k] + W1EQUAL) + Tf, L2A[i - 1][j][k] + W2EQUAL),
					max(max(ATimes[i], L1B[i - 1][j][k] + W1DIFF) + Tf, L2B[i - 1][j][k] + W2EQUAL),
					max(max(ATimes[i], L1Ca[i - 1][j][k] + W1EQUAL) + Tf, L2Ca[i - 1][j][k] + W1DIFF),
					max(max(ATimes[i], L1Cb[i - 1][j][k] + W1DIFF) + Tf, L2Cb[i - 1][j][k] + W1DIFF));
				L2A[i][j][k] = ret.MinNum;
				R2A[i][j][k] = ret.index;
				//cout << "L2A " << i << " " << j << " " << k << " " << L2A[i][j][k] << " " << ret.index << endl;
				//cout << L1B[i][j][k] + Tf << " " << L2B[i - 1][j][k] + W2EQUAL << endl;
				ret = minimum(max(max(BTimes[j], L1A[i][j - 1][k] + W1DIFF) + Tf, L2A[i][j - 1][k] + W2EQUAL),
					max(max(BTimes[j], L1B[i][j - 1][k] + W1EQUAL) + Tf, L2B[i][j - 1][k] + W2EQUAL),
					max(max(BTimes[j], L1Ca[i][j - 1][k] + W1DIFF) + Tf, L2Ca[i][j - 1][k] + W1DIFF),
					max(max(BTimes[j], L1Cb[i][j - 1][k] + W1EQUAL) + Tf, L2Cb[i][j - 1][k] + W1DIFF));
				L2B[i][j][k] = ret.MinNum;
				R2B[i][j][k] = ret.index;
				//cout << "L2B " << i << " " << j << " " << k << " " << L2B[i][j][k] << endl;
				////
				float max1 = max(CTimes[k], L2A[i][j][k - 1] + W2DIFF);
				float max2 = max(CTimes[k], L2Ca[i][j][k - 1] + W2EQUAL);
				L2Ca[i][j][k] = min(max1, max2);
				if (L2Ca[i][j][k] == max1)
					R2Ca[i][j][k] = 0;
				if (L2Ca[i][j][k] == max2)
					R2Ca[i][j][k] = 2;
				//cout << "L2Ca " << i << " " << j << " " << k << " " << L2Ca[i][j][k] << endl;
				///
				max1 = max(CTimes[k], L2B[i][j][k - 1] + W2DIFF);
				max2 = max(CTimes[k], L2Ca[i][j][k - 1] + W2EQUAL);
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
	reverse(TimeListA.begin(), TimeListA.end());
	reverse(TimeListB.begin(), TimeListB.end());
	reverse(TimeListC.begin(), TimeListC.end());
	string TraceOne = TraceRecode;
	TraceOne.erase(remove(TraceOne.begin(), TraceOne.end(), '2'), TraceOne.end());
	IndexI = 1;
	IndexJ = 1;
	char IndexLast = 'a';
	double IndexTime = 0;
	for (char c : TraceOne)
	{
		if (c == '0')
		{
			float w = W1EQUAL;
			if (IndexLast == '0')
				w = W1EQUAL;
			else if (IndexLast == '1')
				w = W1DIFF;
			IndexLast = c;
			TimeListA1.push_back(max(ATimes[IndexI], IndexTime + w));
			IndexTime = TimeListA1.back();
			IndexI++;
		}
		else if (c == '1')
		{
			float w = W1EQUAL;
			if (IndexLast == '0')
				w = W1DIFF;
			else if (IndexLast == '1')
				w = W1EQUAL;
			IndexLast = c;
			TimeListB1.push_back(max(ATimes[IndexJ], IndexTime + w));
			IndexTime = TimeListB1.back();
			IndexJ++;
		}
	}
	FinalTime = ret.MinNum;
	Tdelay = -1;
}
void DoConFIFO()
{
	TimeListA.clear();
	TimeListB.clear();
	TimeListC.clear();
	TimeListA1.clear();
	TimeListB1.clear();
	float W1EQUAL = WEQUAL;
	float W1DIFF = WDIFF;
	float W2EQUAL = WEQUAL;
	float W2DIFF = WDIFF;
	int alpha = ATimes.size() - 1;
	int beta = BTimes.size() - 1;
	int theta = CTimes.size() - 1;
	//
	int IndexI = 1;
	int IndexJ = 1;
	int LastCar = -1;  // 0 mean from a, 1 mean from b
	double TimeIndex = -3.0;
	string TraceOne = "";
	vector<float> TimeListAB;
	while (IndexI < alpha + 1 && IndexJ < beta + 1)
	{
		if (ATimes[IndexI] <= BTimes[IndexJ])
		{
			TraceOne = TraceOne + "0";
			if (LastCar == 0)
				TimeIndex = max(ATimes[IndexI], TimeIndex + W1EQUAL);
			else
				TimeIndex = max(ATimes[IndexI], TimeIndex + W1DIFF);
			TimeListA1.push_back(TimeIndex);
			LastCar = 0;
			IndexI = IndexI + 1;
		}
		else
		{
			TraceOne = TraceOne + "1";
			if (LastCar == 1)
				TimeIndex = max(BTimes[IndexJ], TimeIndex + W1EQUAL);
			else
				TimeIndex = max(BTimes[IndexJ], TimeIndex + W1DIFF);
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
			TimeIndex = max(ATimes[IndexI], TimeIndex + W1EQUAL);
		else
			TimeIndex = max(ATimes[IndexI], TimeIndex + W1DIFF);
		TimeListA1.push_back(TimeIndex);
		TimeListAB.push_back(TimeIndex + Tf);
		LastCar = 0;
		IndexI = IndexI + 1;
	}
	while (IndexJ < beta + 1)
	{
		TraceOne = TraceOne + "1";
		if (LastCar == 1)
			TimeIndex = max(BTimes[IndexJ], TimeIndex + W1EQUAL);
		else
			TimeIndex = max(BTimes[IndexJ], TimeIndex + W1DIFF);
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
	while (IndexI < ApulusB && IndexJ < theta + 1)
	{
		if (TimeListAB[IndexI] <= CTimes[IndexJ])
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
		else
		{
			TraceRecode = TraceRecode + "2";
			if (LastCar == 1)
				TimeIndex = max(CTimes[IndexJ], TimeIndex + W1EQUAL);
			else
				TimeIndex = max(CTimes[IndexJ], TimeIndex + W1DIFF);
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
			TimeIndex = max(CTimes[IndexI], TimeIndex + W1EQUAL);
		else
			TimeIndex = max(CTimes[IndexI], TimeIndex + W1DIFF);
		TimeListC.push_back(TimeIndex);
		LastCar = 1;
		IndexJ++;
	}
	float OutMin = max(TimeListA.back(), TimeListB.back());
	OutMin = max(OutMin, TimeListC.back());
	FinalTime = OutMin;
	Tdelay = -1;
}
int GetTimeOneAns(int LaneNum, float* TimeList)
{
	if (LaneNum == 1)
	{
		for (int i = 0;i < TimeListA1.size();i++)
			TimeList[i] = TimeListA1[i];
		return TimeListA1.size();
	}
	if (LaneNum == 2)
	{
		for (int i = 0;i < TimeListB1.size();i++)
			TimeList[i] = TimeListB1[i];
		return TimeListB1.size();
	}
}
int GetTimeAns(int LaneNum, float* TimeList)
{
	if (LaneNum == 1)
	{
		for (int i = 0;i < TimeListA.size();i++)
			TimeList[i] = TimeListA[i];
		return TimeListA.size();
	}
	if (LaneNum == 2)
	{
		for (int i = 0;i < TimeListB.size();i++)
			TimeList[i] = TimeListB[i];
		return TimeListB.size();
	}
	if (LaneNum == 3)
	{
		for (int i = 0;i < TimeListC.size();i++)
			TimeList[i] = TimeListC[i];
		return TimeListC.size();
	}
}
float GetFinalTime()
{
	return FinalTime;
}
float GetTDelay()
{
	return Tdelay;
}
void ClearAll()
{
	FinalTime = -1;
	Tdelay = -1;
	ATimes.clear();
	BTimes.clear();
	CTimes.clear();
	ARealTimes.clear();
	BRealTimes.clear();
	CRealTimes.clear();
	TimeListA.clear();
	TimeListB.clear();
	TimeListC.clear();
	TimeListA1.clear();
	TimeListB1.clear();
}
void Test(int* arr)
{
    for (int i = 0;i < 10;i++)
        arr[i] = i * 2;
    
}